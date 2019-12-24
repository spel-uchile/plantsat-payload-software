/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2019, Matias Ramirez Martinez, nicoram.mt@gmail.com
 *      Copyright 2019, Carlos Gonzalez Cortes, carlgonz@uchile.cl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "taskInit.h"

static const char *tag = "taskInit";

static csp_iface_t csp_if_kiss;
static csp_iface_t *csp_if_zmqhub;
static csp_kiss_handle_t csp_kiss_driver;
void my_usart_rx(uint8_t * buf, int len, void * pxTaskWoken) {
    csp_kiss_rx(&csp_if_kiss, buf, len, pxTaskWoken);
}

void taskInit(void *param)
{
    /* Initialize subsystem commands */
#if SCH_SEN_ENABLED
    cmd_sensors_init();
#endif

    /* Initialize system variables */
    LOGD(tag, "Initializing system variables values...")
    dat_set_system_var(dat_obc_hrs_wo_reset, 0);
    dat_set_system_var(dat_obc_reset_counter, dat_get_system_var(dat_obc_reset_counter) + 1);
    dat_set_system_var(dat_obc_sw_wdt, 0);  // Reset the gnd wdt on boot
#if (SCH_STORAGE_MODE > 0)
    initialize_payload_vars();
#endif

    LOGD(tag, "Initialization commands ...");
    // Init LibCSP system
    init_communications();

    LOGD(tag, "Creating client tasks ...");
    int t_ok;
    int n_threads = 6;
    os_thread thread_id[n_threads];

    /* Creating clients tasks */
    t_ok = osCreateTask(taskConsole, "console", SCH_TASK_CON_STACK, NULL, 2, &(thread_id[0]));
    if(t_ok != 0) LOGE(tag, "Task console not created!");
#if SCH_SEN_ENABLED
    t_ok = osCreateTask(taskSensors, "sensors", SCH_TASK_HKP_STACK, NULL, 2, &(thread_id[1]));
    if(t_ok != 0) LOGE(tag, "Task sensors not created!");
#endif

#if SCH_HK_ENABLED
    t_ok = osCreateTask(taskHousekeeping, "housekeeping", SCH_TASK_HKP_STACK, NULL, 2, &(thread_id[2]));
    if(t_ok != 0) LOGE(tag, "Task housekeeping not created!");
#endif
#if SCH_COMM_ENABLE
    t_ok = osCreateTask(taskCommunications, "comm", SCH_TASK_COM_STACK, NULL, 2, &(thread_id[3]));
    if(t_ok != 0) LOGE(tag, "Task communications not created!");
#endif
#if SCH_FP_ENABLED
    t_ok = osCreateTask(taskFlightPlan,"flightplan", SCH_TASK_FPL_STACK, NULL, 2, &(thread_id[4]));
    if(t_ok != 0) LOGE(tag, "Task flightplan not created!");
#endif

    osTaskDelete(NULL);
}

void init_communications(void)
{
    /* Init communications */
    LOGI(tag, "Initialising CSP...");

    csp_debug_set_level(CSP_ERROR, 1);
    csp_debug_set_level(CSP_WARN, 1);
    csp_debug_set_level(CSP_INFO, 1);
    csp_debug_set_level(CSP_BUFFER, 0);
    csp_debug_set_level(CSP_PACKET, 0);
    csp_debug_set_level(CSP_PROTOCOL, 0);
    csp_debug_set_level(CSP_LOCK, 0);

    /* Init buffer system */
    int t_ok;
    t_ok = csp_buffer_init(SCH_BUFFERS_CSP, SCH_BUFF_MAX_LEN);
    if(t_ok != 0) LOGE(tag, "csp_buffer_init failed!");

    /* Init CSP */
    csp_set_hostname(SCH_NAME);
    csp_init(SCH_COMM_ADDRESS); // Init CSP with address MY_ADDRESS

    /**
     * Set interfaces and routes
     *  Platform dependent
     */
    /* Set ZMQ interface as a default route*/
    /* TODO: Add I2C interface */
    uint8_t addr = (uint8_t)SCH_COMM_ADDRESS;
    uint8_t *rxfilter = &addr;
    unsigned  int rxfilter_count = 1;

    csp_zmqhub_init_w_name_endpoints_rxfilter(CSP_ZMQHUB_IF_NAME,
                                              rxfilter, rxfilter_count,
                                              SCH_COMM_ZMQ_OUT, SCH_COMM_ZMQ_IN,
                                              &csp_if_zmqhub);
    csp_route_set(CSP_DEFAULT_ROUTE, csp_if_zmqhub, CSP_NODE_MAC);

    /* Start router task with SCH_TASK_CSP_STACK word stack, OS task priority 1 */
    t_ok = csp_route_start_task(SCH_TASK_CSP_STACK, 1);
    if(t_ok != 0) LOGE(tag, "Task router not created!");

    LOGI(tag, "Route table");
    csp_route_print_table();
    LOGI(tag, "Interfaces");
    csp_route_print_interfaces();
}

void init_routines(void)
{

}
