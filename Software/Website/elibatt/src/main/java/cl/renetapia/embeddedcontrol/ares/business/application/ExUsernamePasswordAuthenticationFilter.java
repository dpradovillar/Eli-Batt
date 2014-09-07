package cl.renetapia.embeddedcontrol.ares.business.application;

import cl.renetapia.embeddedcontrol.ares.business.application.captcha.CageCaptchaServlet;
import cl.renetapia.embeddedcontrol.ares.business.application.captcha.CaptchaSingleton;
import org.apache.log4j.Logger;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 02-07-13
 * Time: 08:49 PM
 * To change this template use File | Settings | File Templates.
 */
public class ExUsernamePasswordAuthenticationFilter extends UsernamePasswordAuthenticationFilter {

    private static Logger logger = Logger.getLogger(ExUsernamePasswordAuthenticationFilter.class);

    @Override
    public Authentication attemptAuthentication(HttpServletRequest request, HttpServletResponse response) throws AuthenticationException {
        HttpSession session = request.getSession(true); // get session

        String errorMsg = null;

        // get previously stored response (set by CageCaptchaServlet)
        Object objCaptchaResponse = session.getAttribute(CageCaptchaServlet.CAPTCHA_RESPONSE);

        String captchaCode = request.getParameter(CageCaptchaServlet.CAPTCHA_CODE); // input from form submit
        String captchaResponse = (objCaptchaResponse == null ? null : String.valueOf(objCaptchaResponse));

        // perform validation of captcha
        if (captchaCode == null || captchaResponse == null) {
            errorMsg = "Invalid session.";
        } else if (!captchaCode.equals(captchaResponse)) {
            errorMsg = "Invalid captcha.";
        }
        logger.info("captcha errorMsg:" + errorMsg);

        // store status of validation for later exception
        ThreadLocal<String> threadLocal = CaptchaSingleton.getInstance().getThreadLocal();
        //threadLocal.set(errorMsg);

        return super.attemptAuthentication(request, response);
    }
}