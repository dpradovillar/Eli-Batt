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
@RequestMapping("/data")
public class DataController extends BaseController {

    private static Logger logger = Logger.getLogger(DataController.class);

    public DataController() {
        super(1);
    }

    @RequestMapping("/index.html")
    public ModelAndView index() {
        return ms();
    }

    @RequestMapping("/raw_data.html")
    public ModelAndView raw_data() {
        return ms(1);
    }

    @RequestMapping("/graphs.html")
    public ModelAndView graphs() {
        return ms(2);
    }

    @RequestMapping("/reports.html")
    public ModelAndView reports() {
        return ms(3);
    }
}