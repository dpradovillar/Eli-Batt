package cl.renetapia.embeddedcontrol.ares.business.application.captcha;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 02-07-13
 * Time: 11:14 PM
 * To change this template use File | Settings | File Templates.
 */
public class CaptchaSingleton {

    private static CaptchaSingleton instance;

    private CaptchaSingleton(){

    }

    public static CaptchaSingleton getInstance() {
        if (instance == null) {
            instance = new CaptchaSingleton();
        }
        return instance;
    }

    private final ThreadLocal<String> local = new ThreadLocal<String>() {
        @Override protected String initialValue() {
            return null;
        }
    };

    public ThreadLocal<String> getThreadLocal() {
        return local;
    }

}
