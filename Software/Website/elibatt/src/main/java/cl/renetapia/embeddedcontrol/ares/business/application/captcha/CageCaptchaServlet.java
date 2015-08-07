package cl.renetapia.embeddedcontrol.ares.business.application.captcha;

import com.github.cage.Cage;
import com.github.cage.GCage;
import org.apache.log4j.Logger;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 02-07-13
 * Time: 08:34 PM
 * To change this template use File | Settings | File Templates.
 */
public class CageCaptchaServlet extends HttpServlet {

    public static final String CAPTCHA_CODE = "CAPTCHA_CODE";
    public static final String CAPTCHA_RESPONSE = "CAPTCHA_RESPONSE";
    public static final String CAPTCHA_TOKEN_USED = "CAPTCHA_TOKEN_USED";
    public static final String CAPTCHA_TOKEN = "CAPTCHA_TOKEN";
    public static final String CAPTCHA_RESET = "CAPTCHA_RESET";
//    public static final String CAPTCHA_TIMESTAMP = "CAPTCHA_TIMESTAMP";

    private static Logger logger = Logger.getLogger(CageCaptchaServlet.class);

    private static final long serialVersionUID = 1490947492185481844L;

    private static final Cage cage = new GCage();

    public static void setToken(HttpSession session, String token) {
        session.setAttribute(CAPTCHA_TOKEN, token);
    }

    /**
     * Generates a captcha token and stores it in the session.
     *
     * @param session where to store the captcha.
     */
    public static String generateToken(HttpSession session) {

        String token = cage.getTokenGenerator().next();
        setToken(session, token);
        markTokenUsed(session, false);
        return token;
    }

    /**
     * Used to retrieve previously stored captcha token from session.
     *
     * @param session where the token is possibly stored.
     * @return token or null if there was none
     */
    public static String getToken(HttpSession session) {
        Object val = session.getAttribute(CAPTCHA_TOKEN);
        return val != null ? val.toString() : null;
    }

    /**
     * Marks token as used/unused for image generation.
     *
     * @param session where the token usage flag is possibly stored.
     * @param used    false if the token is not yet used for image generation
     */
    protected static void markTokenUsed(HttpSession session, boolean used) {
        session.setAttribute(CAPTCHA_TOKEN_USED, used);
    }

    /**
     * Checks if the token was used/unused for image generation.
     *
     * @param session where the token usage flag is possibly stored.
     * @return true if the token was marked as unused in the session
     */
    protected static boolean isTokenUsed(HttpSession session) {
        return !Boolean.FALSE.equals(session.getAttribute(CAPTCHA_TOKEN_USED));
    }

    public static void resetToken(HttpServletRequest req) {
        HttpSession session = req.getSession();
        generateToken(session);
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        HttpSession session = req.getSession(true);

        // si es primera peticion
        resetToken(req);
        markTokenUsed(session, true);

        String token = getToken(session);

        session.setAttribute(CAPTCHA_RESPONSE, token);

        //logger.info("drawing captcha image, token=" + token);

        setResponseHeaders(resp);

        cage.draw(token, resp.getOutputStream());
    }

    /**
     * Helper method, disables HTTP caching.
     *
     * @param resp response object to be modified
     */
    protected void setResponseHeaders(HttpServletResponse resp) {
        resp.setContentType("image/" + cage.getFormat());
        resp.setHeader("Cache-Control", "no-cache, no-store");
        resp.setHeader("Pragma", "no-cache");
        long time = System.currentTimeMillis();
        resp.setDateHeader("Last-Modified", time);
        resp.setDateHeader("Date", time);
        resp.setDateHeader("Expires", time);
    }
}