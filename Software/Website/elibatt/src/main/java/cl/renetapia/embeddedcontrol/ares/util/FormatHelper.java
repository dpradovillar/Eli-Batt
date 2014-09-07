package cl.renetapia.embeddedcontrol.ares.util;

import org.apache.log4j.Logger;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 30-06-13
 * Time: 04:11 AM
 * To change this template use File | Settings | File Templates.
 */
public class FormatHelper {

    private static Logger logger = Logger.getLogger(FormatHelper.class);

    public static String formatCurrency(int value) {

        /*BigDecimal bd = new BigDecimal(value);
        DecimalFormatSymbols symbols = DecimalFormatSymbols.getInstance();
        DecimalFormat formatter = new DecimalFormat("###.###", symbols);
        return "$" + formatter.format(bd.longValue());
        */

        StringBuffer sb = new StringBuffer();
        int count = 0;
        boolean appendSign = false;
        if (value != 0) {
            if (value < 0) {
                appendSign = true;
                value = -value;
            }
            while (value > 0) {
                int lastDig = value % 10;
                value /= 10;
                sb.append(String.valueOf(lastDig));
                if (++count == 3 && value > 0) {
                    sb.append('.');
                    count = 0;
                }
            }
        } else {
            sb.append('0');
        }
        return "$" + (appendSign ? "-" : "") + sb.reverse().toString() + ".-";
    }

    public static int parseCurrency(String value) {
        return Integer.parseInt(value.replace("\\.", "").replace("\\$", ""));
    }

    private static final SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy");

    public static String formatDate(Date date) {
        return sdf.format(date);
    }

    public static Date parseDate(String strDate) {
        try {
            return sdf.parse(strDate);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            throw new RuntimeException(e);
        }
    }

    public static String convertDateFormat(String inputDate, String inputFormat, String outputFormat) {
        try {
            SimpleDateFormat sdf = new SimpleDateFormat(inputFormat);
            Date theDate = sdf.parse(inputDate);
            SimpleDateFormat sdf2 = new SimpleDateFormat(outputFormat);
            return sdf2.format(theDate);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            throw new RuntimeException(e);
        }
    }
}
