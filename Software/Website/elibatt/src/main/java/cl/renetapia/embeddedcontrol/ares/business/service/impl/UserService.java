package cl.renetapia.embeddedcontrol.ares.business.service.impl;

import cl.renetapia.embeddedcontrol.ares.business.service.IUserService;
import cl.renetapia.embeddedcontrol.ares.dao.IUserDAO;
import cl.renetapia.embeddedcontrol.ares.entity.UserEntity;
import org.springframework.stereotype.Repository;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.annotation.Resource;
import java.util.List;

@Service
@Repository
public class UserService implements IUserService {

    @Resource
    private IUserDAO userDAO;


    @Override
    @Transactional
    public UserEntity findByUsername(String username) {
        return userDAO.findByUsername(username);
    }

    @Override
    @Transactional
    public List<UserEntity> findAll() {
        return userDAO.findAll();
    }
}
