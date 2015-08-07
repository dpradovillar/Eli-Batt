package cl.renetapia.embeddedcontrol.ares.controller;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 22-06-13
 * Time: 02:20 AM
 * To change this template use File | Settings | File Templates.
 */

import cl.renetapia.embeddedcontrol.ares.business.application.ApplicationParameters;
import cl.renetapia.embeddedcontrol.ares.controller.base.BaseController;
import org.apache.log4j.Logger;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

@Controller
@RequestMapping("/main")
public class MainController extends BaseController {

    private static Logger logger = Logger.getLogger(MainController.class);

    public MainController() {
        super(0);
    }

    @RequestMapping("/index.html")
    public ModelAndView index() {
        ApplicationParameters ap = ApplicationParameters.getInstance();
        return ms()
                .addObject("working_location", ap.getWorkingLocation())
                .addObject("downloads_location", ap.getDownloadsLocation());
    }

}