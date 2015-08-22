#ifndef LPC_CONFIGURATION_H_
#define LPC_CONFIGURATION_H_

/* Definition of argument values for the configure_*() efuns.
 */

/* Possible options for configure_interactive().
 */
#define IC_MAX_WRITE_BUFFER_SIZE 0

/* Possible options for configure_opbject().
 */
#define OC_COMMANDS_ENABLED    0
#define OC_HEART_BEAT          1

/* Possible options for configure_driver().
 */
#define DC_MEMORY_LIMIT        0
#define DC_ENABLE_HEART_BEATS  1
#define DC_LONG_EXEC_TIME      2
#define DC_DATA_CLEAN_TIME     3
#define DC_TLS_CERTIFICATE     4

#endif /* LPC_CONFIGURATION_H_ */
