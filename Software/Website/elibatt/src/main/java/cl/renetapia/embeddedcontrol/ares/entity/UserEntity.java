package cl.renetapia.embeddedcontrol.ares.entity;

import cl.renetapia.embeddedcontrol.ares.entity.base.BaseEntity;

import javax.persistence.*;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 01-11-13
 * Time: 04:46 PM
 * To change this template use File | Settings | File Templates.
 */
@Table(name = "ares_users")
@Entity
public class UserEntity extends BaseEntity {

    private Integer id;
    private String username;
    private String password;
    
    @Column(name = "id", nullable = false, insertable = true, updatable = true, length = 10, precision = 0)
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Id
    public Integer getId() {return id;}
    public void setId(Integer id) {this.id = id;}

    @Column(name = "username", nullable = false, insertable = true, updatable = true, length = 255, precision = 0)
    @Basic
    public String getUsername() {return username;}
    public void setUsername(String username) {this.username = username;}
    

    @Column(name = "password", nullable = false, insertable = true, updatable = true, length = 255, precision = 0)
    @Basic
    public String getPassword() {return password;}
    public void setPassword(String password) {this.password = password;}

}
