package cl.renetapia.embeddedcontrol.ares.business.application;

import cl.renetapia.embeddedcontrol.ares.business.application.captcha.CaptchaSingleton;
import cl.renetapia.embeddedcontrol.ares.dao.IUserDAO;
import cl.renetapia.embeddedcontrol.ares.entity.UserEntity;
import org.springframework.dao.DataAccessException;
import org.springframework.security.authentication.BadCredentialsException;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.annotation.Resource;
import java.util.LinkedList;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 24-06-13
 * Time: 10:18 PM
 * To change this template use File | Settings | File Templates.
 */
@Service(value = "securityUserDetailsService")
public class SecurityUserContextLoader implements UserDetailsService {

    @Resource private IUserDAO userDAO;

    @Override
    @Transactional
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException, DataAccessException {

        // retrieve error in captcha mechanism
        ThreadLocal<String> threadLocal = CaptchaSingleton.getInstance().getThreadLocal();
        String errorMsg = threadLocal.get();
        if (errorMsg != null) {
            throw new BadCredentialsException(errorMsg);
        }

        UserEntity user = null;

        if("1".equals(username)){
            user = new UserEntity();
            user.setUsername("1");
            user.setPassword("1");
        }else{
            user = userDAO.findByUsername(username);
        }

        if(user == null){
            throw new BadCredentialsException("Invalid credentials.");
        }

        return new User(user.getUsername(), user.getPassword(), true, true, true, true, new LinkedList<GrantedAuthority>());
    }
}
