package cl.renetapia.embeddedcontrol.ares.business.validator;

import org.apache.log4j.Logger;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 07-05-13
 * Time: 02:51 PM
 * To change this template use File | Settings | File Templates.
 */
public class Validator {

    private static final Logger logger = Logger.getLogger(Validator.class);

    public static void shouldNotBeNull(Object o) {
        if (o == null) {
            throw new BusinessException("Null value is not allowed.");
        }
    }

    public static void shouldBeNull(Object o) {
        if (o != null) {
            throw new BusinessException("Value should not be set (null was expected)");
        }
    }

    public static void shouldNotBeNegative(int val) {
        if (val < 0) {
            throw new BusinessException("Value cannot be negative (" + val + ")");
        }
    }

    public static void shouldBeFound(Object o) {
        shouldBeFound(o, new BusinessException("Data could not be found"));
    }

    public static void shouldBeFound(Object o, RuntimeException e) {
        if (o == null) {
            throw e;
        }
    }

    public static void shouldNotBeFound(Object o) { // same as shouldBeNull
        if (o != null) {
            throw new BusinessException("Data should not be found");
        }
    }

    public static <T extends Comparable<T>> void shouldBeOrdered(T... args) {
        for (int i = 1; i < args.length; i++) {
            T c1 = args[i - 1];
            T c2 = args[i];
            if (c1.compareTo(c2) > 0) {
                throw new BusinessException(c1.toString() + " > " + c2.toString());
            }
        }
    }

    public static void shouldNotBeEmpty(String username) {
        if (username == null || username.isEmpty()) {
            throw new BusinessException("Value cannot be empty.");
        }
    }

    public static void shouldBeNumeric(String numeric) {
        try {
            int val = Integer.parseInt(String.valueOf(numeric));
        } catch (Exception e) {
            throw new BusinessException("Value should be numeric (integer).");
        }
    }

    public static void shouldBeEquals(Object a, Object b) {
        if (a == null) {
            shouldBeNull(b);
        }
        if (b == null) {
            shouldBeNull(a);
        }
        if (!a.equals(b)) {
            throw new BusinessException("Values should be equal.");
        }
    }

    public static void shouldBeUsername(String username) {
        shouldNotBeNull(username);
        if (!username.matches("[a-zA-Z0-9_]+")) {
            throw new BusinessException("Username can only contain letters lowercase/uppercase, digits and underscore.");
        }
    }


    public static void shouldBeBooleanStr(String hired) {
        boolean cond = "true".equals(hired) || "false".equals(hired);
        if (!cond) {
            throw new BusinessException("error, boolean value required (true|false): invalid value \"" + hired + "\"");
        }
        // ok
    }

    public static void shouldBePhoneNumber(String phoneNumStr) {
        shouldNotBeEmpty(phoneNumStr);
        if (!phoneNumStr.matches("\\d{7,12}")) {
            throw new BusinessException("Phone numbers can only contain between 7 and 12 digits.");
        }
    }

    public static void shouldBeEmail(String emailStr) {
        shouldNotBeEmpty(emailStr);
        if (!emailStr.matches("(.+)@(.+)\\.(.+)")) {
            throw new BusinessException("Invalid email (" + emailStr + ")");
        }
    }
}
