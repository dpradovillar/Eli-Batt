package cl.renetapia.embeddedcontrol.ares.util;

import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.User;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 03-11-13
 * Time: 05:32 PM
 * To change this template use File | Settings | File Templates.
 */
public class SpringSecurityUtils {

    public static String getLoggedUsername() {
        Object x = SecurityContextHolder.getContext().getAuthentication().getPrincipal();
        if (x instanceof String) {
            return String.valueOf(x);
        }
        return ((User) x).getUsername();
    }

}
