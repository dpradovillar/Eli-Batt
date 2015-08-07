package cl.renetapia.embeddedcontrol.ares.controller;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 22-06-13
 * Time: 02:20 AM
 * To change this template use File | Settings | File Templates.
 */

import cl.renetapia.embeddedcontrol.ares.controller.base.BaseController;
import org.apache.log4j.Logger;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

@Controller
@RequestMapping("/service")
public class ServiceController extends BaseController {

    private static Logger logger = Logger.getLogger(ServiceController.class);

    public ServiceController() {
        super(2);
    }

    @RequestMapping("/index.html")
    public ModelAndView index() {
        return ms();
    }

}