#include "nfl.h"
#include "face-table.h"
#include "app.h"
#include "netif.h"
#include "l2.h"
#include "pit.h"
#include "fib.h"
#include "cs.h"
#include "forwarding-strategy.h"
#include "encoding/ndn-constants.h"
#include "encoding/name.h"
#include "encoding/interest.h"
#include "msg-type.h"

#define ENABLE_DEBUG 1
#include <debug.h>
#include <net/gnrc/netapi.h>
#include <net/gnrc/netif.h>
#include <net/gnrc/netreg.h>
#include <thread.h>
#include <timex.h>
#include <xtimer.h>

#define NFL_STACK_SIZE        (THREAD_STACKSIZE_DEFAULT)
#define NFL_PRIO              (THREAD_PRIORITY_MAIN - 3)
#define NFL_MSG_QUEUE_SIZE    (8U)

#if ENABLE_DEBUG
static char _stack[NFL_STACK_SIZE + THREAD_EXTRA_STACKSIZE_PRINTF];
#else
static char _stack[NFL_STACK_SIZE];
#endif

kernel_pid_t nfl_pid = KERNEL_PID_UNDEF;

static int nfl_bootstrap_init()
{
    //these info are static allocated, others all can be deleted after bootstrap
    bootstrap_cb = //callback here
    ndn_block_t* m_Certificate;
    ndn_block_t* m_anchorCert;
    ndn_block_t* m_homePrefix;
    ndn_block_t* m_host;
    uint8_t BKpub[64];
    uint8_t BKpri[32];
    kernel_pid_t nfl_bootstrap_pid = KERNEL_PID_UNDEF;
    char bootstrap_stack[THREAD_STACKSIZE_MAIN];

}

static int nfl_service_discovery_init()
{
    
}
static int _start_bootstrap(void* ptr)
{
    //ptr pointed to a struct have three component: BKpub, BKpri, m_host
    
    //assign value
    msg_t send, reply;
    reply.content.ptr = NULL;
    BKpub = ptr->BKpub;
    BKpri = ptr->BKpri;
    nfl_bootstrap_pid = thread_create(bootstrap_stack, sizeof(bootstrap_stack),
                            THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, ndn_bootstrap, NULL, "nfl-bootstrap");
    //this thread directly registerd on ndn core thread as a application
    send.content.ptr = reply.content.ptr;

    uint32_t seconds = 5;
    xtimer_sleep(seconds);
    msg_send_receive(&send, &reply, pid);

    ndn_block_t* cert;
    ndn_block_t name;
    cert = reply.content.ptr;
    ndn_data_get_name(cert, &name);
    DPRINT("certificate ipc received, name=");
    ndn_name_print(&name);
    putchar('\n');
}

/* Main event loop for NFL */
static void *_event_loop(void *args)
{
    msg_t msg, reply, msg_q[NFL_MSG_QUEUE_SIZE];
    gnrc_netreg_entry_t me_reg;

    (void)args;
    msg_init_queue(msg_q, NFL_MSG_QUEUE_SIZE);

    //TODO: initialize the NFL here

    /* start event loop */
    while (1) {
        msg_receive(&msg);

        switch (msg.type) {
            case NFL_START_BOOTSTRAP:
                DEBUG("NFL: START_BOOTSTRAP message received from pid %"
                      PRIkernel_pid "\n", msg.sender_pid);
                
                _start_bootstrap(msg.content.ptr);

                //ndn_pit_timeout((msg_t*)msg.content.ptr);
                break;

            case NFL_START_SELF_LEARNING:
                DEBUG("NFL: START_SELF_LEARNING message received from pid %"
                      PRIkernel_pid "\n", msg.sender_pid);
                //_start_self_learning(msg.content.ptr);

                //ndn_l2_frag_timeout((msg_t*)msg.content.ptr);
                break;

            case NFL_EXTRACT_ANCHOR_CERT:
                DEBUG("NFL: EXTRACT_ANCHOR_CERT message received from pid %"
                      PRIkernel_pid "\n", msg.sender_pid);
                /*if (ndn_face_table_add(
                        (kernel_pid_t)msg.content.value, NDN_FACE_APP) != 0) {
                    DEBUG("ndn: failed to add face id %d\n",
                          (int)msg.content.value);
                    reply.content.value = 1;
                } else {
                    reply.content.value = 0;  // indicate success
                }
                msg_reply(&msg, &reply);*/
                break;

            case NFL_EXTRACT_M_CERT:
                DEBUG("NFL: EXTRACT_M_CERT message received from pid %"
                      PRIkernel_pid "\n", msg.sender_pid);
                /*if (ndn_face_table_remove(
                        (kernel_pid_t)msg.content.value) != 0) {
                    DEBUG("ndn: failed to remove face id %d\n",
                          (int)msg.content.value);
                    reply.content.value = 1;
                } else {
                    reply.content.value = 0;  // indicate success
                }
                msg_reply(&msg, &reply);*/
                break;

            case NFL_EXTRACT_SERVICE_LIST:
                DEBUG("NFL: EXTRACT_SERVICE_LIST message received from pid %"
                      PRIkernel_pid "\n", msg.sender_pid);
                /*if (ndn_fib_add((ndn_shared_block_t*)msg.content.ptr,
                                msg.sender_pid,
                                NDN_FACE_APP) != 0) {
                    DEBUG("ndn: failed to add fib entry\n");
                    ndn_shared_block_release(
                        (ndn_shared_block_t*)msg.content.ptr);
                    reply.content.value = 1;
                } else {
                    reply.content.value = 0;  // indicate success
                }
                msg_reply(&msg, &reply);*/
                break;

            case NFL_EXTRACT_HOME_PREFIX:
                DEBUG("NFL: EXTRACT_HOME_PREFIX messages received from pid %"
                    PRIkernel_pid "\n", msg.sender_pid);
                /*struct _ndn_app_add_strategy_param* param =
                    (struct _ndn_app_add_strategy_param*)msg.content.ptr;
                if (ndn_forwarding_strategy_add(param->prefix,
                                param->strategy) != 0) {
                    DEBUG("ndn: failed to add forwarding strategy\n");
                    ndn_shared_block_release(param->prefix);
                    reply.content.value = 1;
                } else {
                    reply.content.value = 0;
                }
                msg_reply(&msg, &reply);*/
                break;

            default:
                break;
        }
    }

    return NULL;
}


kernel_pid_t nfl_init(void)
{
    nfl_bootstrap_init();
    nfl_service_discovery_init();

    /* check if thread is already running */
    if (nfl_pid == KERNEL_PID_UNDEF) {
        /* start UDP thread */
        nfl_pid = thread_create(
            _stack, sizeof(_stack), GNRC_NDN_PRIO,
            THREAD_CREATE_STACKTEST, _event_loop, NULL, "NFL");
    }
    return nfl_pid;
}

/** @} */
