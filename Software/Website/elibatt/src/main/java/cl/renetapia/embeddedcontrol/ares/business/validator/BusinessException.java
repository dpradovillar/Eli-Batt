package cl.renetapia.embeddedcontrol.ares.business.validator;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 01-11-13
 * Time: 01:54 PM
 * To change this template use File | Settings | File Templates.
 */
public class BusinessException extends RuntimeException {
    public BusinessException(String s) {
        super(s);
    }
}
