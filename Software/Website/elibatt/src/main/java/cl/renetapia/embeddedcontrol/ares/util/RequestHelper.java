package cl.renetapia.embeddedcontrol.ares.util;

import javax.servlet.http.HttpServletRequest;
import java.util.ArrayList;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 23-06-13
 * Time: 04:32 AM
 * To change this template use File | Settings | File Templates.
 */
public class RequestHelper {

    public static String active(int current, int expected, String class2output) {
        if (current == expected) {
            return " class=\"" + class2output + "\" ";
        }
        return "";
    }

    public static int getInt(HttpServletRequest request, String attributeName) {
        Object o = request.getAttribute(attributeName);
        return (o instanceof Integer ? (Integer) o : 0);
    }

    public static String getString(HttpServletRequest request, String attributeName) {
        return getString(request, attributeName, "");
    }

    public static String getString(HttpServletRequest request, String attributeName, String _default) {
        Object o = request.getAttribute(attributeName);
        return (o == null ? _default : o.toString());
    }

}
