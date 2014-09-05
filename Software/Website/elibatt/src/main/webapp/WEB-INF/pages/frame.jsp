<%@ page import="cl.renetapia.embeddedcontrol.ares.util.RequestHelper" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>Elibatt :: internal site</title>

    <!-- CSS -->
    <link href="../../style/css/transdmin.css" rel="stylesheet" type="text/css" media="screen"/>
    <!--[if IE 6]>
    <link rel="stylesheet" type="text/css" media="screen" href="../../style/css/ie6.css"/><![endif]-->
    <!--[if IE 7]>
    <link rel="stylesheet" type="text/css" media="screen" href="../../style/css/ie7.css"/><![endif]-->

    <link rel="stylesheet" type="text/css" media="screen" href="../../style/css/app.css"/>

    <!-- JavaScripts-->
    <script type="text/javascript" src="../../style/js/jquery.js"></script>
    <script type="text/javascript" src="../../style/js/jNice.js"></script>

    <link type="text/css" rel="stylesheet" href="../../style/css/zTreeStyle/zTreeStyle.css"/>
    <script type="text/javascript" src="../../style/js/jquery-1.4.4.min.js"></script>
    <script type="text/javascript" src="../../style/js/jquery.ztree.core-3.5.js"></script>
    <script type="text/javascript" src="../../style/js/jquery.ztree.exedit-3.5.js"></script>

</head>

<body>

<div id="wrapper">

    <jsp:include page="frame/header.jsp"/>

    <jsp:include page="frame/menu.jsp"/>

    <div id="containerHolder">
        <div id="container">

            <jsp:include page="frame/sidebar.jsp"/>

            <% int m = RequestHelper.getInt(request, "module"); %>

            <%
                switch (m) {
                    case -1: %>
            <jsp:include page="frame/error.jsp"/>
            <% break;
                case 0: %>
            <jsp:include page="frame/main.jsp"/>
            <% break;
                case 1: %>
            <jsp:include page="data.jsp"/>
            <% break;
                case 2: %>
            <jsp:include page="service.jsp"/>
            <% break;
                case 3: %>
            <jsp:include page="user.jsp"/>
            <% break;
            }
            %>

            <div class="clear"></div>
        </div>
        <!-- // #container -->
    </div>
    <!-- // #containerHolder -->


</div>
<!-- end of wrapper -->

<jsp:include page="frame/footer.jsp"/>


</div>
<!-- // #wrapper -->

</body>
</html>