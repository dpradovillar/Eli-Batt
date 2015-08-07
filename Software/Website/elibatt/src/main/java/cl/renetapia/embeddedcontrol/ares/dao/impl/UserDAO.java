package cl.renetapia.embeddedcontrol.ares.dao.impl;

import cl.renetapia.embeddedcontrol.ares.dao.IUserDAO;
import cl.renetapia.embeddedcontrol.ares.entity.UserEntity;
import org.springframework.stereotype.Repository;

@Repository
public class UserDAO extends BaseDAO<UserEntity, Integer> implements IUserDAO {

    public UserDAO() {
        super(UserEntity.class);
    }

    @Override
    public UserEntity findByUsername(String username) {
        return findOneByStatement("select u from UserEntity u where u.username = :username",
                "username", username
        );
    }
}
