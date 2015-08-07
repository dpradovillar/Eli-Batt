<%@ page contentType="text/html; charset=utf-8" %>
<%@ page import="cl.renetapia.embeddedcontrol.ares.util.RequestHelper" %>
<%@ page import="cl.renetapia.embeddedcontrol.ares.util.SpringSecurityUtils" %>

<!-- You can name the links with lowercase, they will be transformed to uppercase by CSS, we prefered to name them with uppercase to have the same effect with disabled stylesheet -->

<div class="welcome">Welcome <b><%= SpringSecurityUtils.getLoggedUsername() %></b></div>

<br clear="right"/>

<% int m = RequestHelper.getInt(request, "module"); %>

<ul id="mainNav">
    <li><a href="../data/index.html"<%= RequestHelper.active(m, 1, "active") %>>DATA</a></li>
    <li><a style="color:lightgray;" href="#"<%= RequestHelper.active(m, 2, "active") %>>SERVICES</a></li>
    <li><a href="../user/index.html"<%= RequestHelper.active(m, 3,"active") %>>USERS</a></li>
    <li class="logout"><a href="../../logout">LOGOUT</a></li>
</ul>

</span>
<!-- // #end mainNav -->

