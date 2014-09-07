package cl.renetapia.embeddedcontrol.ares.controller;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 22-06-13
 * Time: 02:20 AM
 * To change this template use File | Settings | File Templates.
 */

import cl.renetapia.embeddedcontrol.ares.business.service.IUserService;
import cl.renetapia.embeddedcontrol.ares.controller.base.BaseController;
import cl.renetapia.embeddedcontrol.ares.entity.UserEntity;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import java.util.List;

@Controller
@RequestMapping("/user")
public class UserController extends BaseController {

    private static Logger logger = Logger.getLogger(UserController.class);

    @Autowired private IUserService userService;

    public UserController() {
        super(3);
    }

    @RequestMapping("/index.html")
    public ModelAndView index() {
        return ms();
    }

    @RequestMapping("/view_all.html")
    public ModelAndView view_all() {
        List<UserEntity> users = userService.findAll();
        return ms(1)
                .addObject("users", users)
                ;
    }

    @RequestMapping("/add_new.html")
    public ModelAndView add_new() {
        return ms(2);
    }

    @RequestMapping("/roles.html")
    public ModelAndView roles() {
        return ms(3);
    }

}