/* This sefun is to provide a replacement for the efuns query_ip_name() and
 * query_ip_number().
 * Feel free to add it to your mudlibs, if you have much code relying on that.
 */

#if ! __EFUN_DEFINED__(query_ip_name)

#include <interactive_info.h>

string query_ip_name(object player)
{
    object ob = player;
    ob ||= efun::this_player();

    player = efun::interactive_info(ob, II_IP_ADDRESS);
    return efun::interactive_info(ob, II_IP_NAME);
}

string query_ip_number(object player)
{
    object ob = player;
    ob ||= efun::this_player();

    player = efun::interactive_info(ob, II_IP_ADDRESS);
    return efun::interactive_info(ob, II_IP_NUMBER);
}

#endif
