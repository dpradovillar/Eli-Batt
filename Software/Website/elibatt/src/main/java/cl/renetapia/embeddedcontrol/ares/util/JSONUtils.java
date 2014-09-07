package cl.renetapia.embeddedcontrol.ares.util;

import org.json.simple.JSONObject;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 03-11-13
 * Time: 06:17 PM
 * To change this template use File | Settings | File Templates.
 */
public class JSONUtils {

    public static String singleValue(String key, boolean value) {
        JSONObject obj = new JSONObject();
        obj.put(key, value);
        return obj.toJSONString();
    }

    public static String singleValue(String key, int value) {
        JSONObject obj = new JSONObject();
        obj.put(key, value);
        return obj.toJSONString();
    }

}
