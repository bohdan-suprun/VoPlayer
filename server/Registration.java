//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

package com.voplayer;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.net.ConnectException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.NoSuchElementException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.commons.codec.binary.Base64;

public class Registration extends HttpServlet {
    String name;
    String email;
    String hash;
    boolean isConnected;
    Connection connection = null;
    Statement st;

    public Registration() {
    }

    public void init() {
        try {
            Class.forName("com.mysql.jdbc.Driver");
            this.connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/voplayer?useUnicode=true&characterEncoding=UTF-8", "matroskin", "voplayeruser");
            if(this.connection != null) {
                this.isConnected = true;
                this.st = this.connection.createStatement();
            }
        } catch (ClassNotFoundException var2) {
            this.isConnected = false;
        } catch (SQLException var3) {
            this.isConnected = false;
        }

    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        response.setContentType("application/xml; charset=UTF-8");
        response.setCharacterEncoding("UTF-8");
        PrintWriter out = response.getWriter();

        try {
            if(!this.isConnected) {
                throw new ConnectException("Can\'t connect to database");
            }

            int ex = Integer.valueOf(request.getParameter("action")).intValue();
            out.println("<xml><action>" + ex + "</action>");
            switch(ex) {
            case 0:
                this.name = this.decrypt(request.getParameter("name")).trim();
                this.email = this.decrypt(request.getParameter("email"));
                out.println(this.registration(request.getParameter("pass")));
                out.println("</xml>");
                break;
            case 1:
                this.email = this.decrypt(request.getParameter("email"));
                out.println(this.logIn(request.getParameter("mac"), request.getParameter("pass")));
                break;
            case 2:
                this.hash = request.getParameter("hash");
                this.isSecure(request.getParameter("mac"));
                this.write(request.getParameter("en").trim(), request.getParameter("ru").trim());
                out.println("</xml>");
                break;
            case 3:
                this.hash = request.getParameter("hash");
                this.logOut(request.getParameter("mac"));
                out.println("</xml>");
                break;
            case 4:
                this.hash = request.getParameter("hash");
                this.isSecure(request.getParameter("mac"));
                out.println(this.read(Integer.valueOf(request.getParameter("number")).intValue()));
                break;
            case 5:
                this.hash = request.getParameter("hash");
                this.isSecure(request.getParameter("mac"));
                out.println(this.logViaHash());
                break;
            case 6:
                this.hash = request.getParameter("hash");
                this.isSecure(request.getParameter("mac"));
                out.println(this.info());
                break;
            default:
                out.println("<message>Unknown action</message></xml>");
            }
        } catch (SecurityException var5) {
            out.print("<message>Request can\'t be handled. Please, log in or register</message></xml>");
        } catch (ConnectException var6) {
            out.print("<message>Internal error</message></xml>");
        }

    }

    private String crypt(byte[] str) {
        return Base64.encodeBase64URLSafeString(str);
    }

    private String decrypt(String str) {
        try {
            return new String(Base64.decodeBase64(str), "UTF-8");
        } catch (UnsupportedEncodingException var3) {
            return null;
        }
    }

    private String registration(String pass) {
        String result = "";

        try {
            String ex = "";
            new Base64(true);
            MessageDigest key = MessageDigest.getInstance("MD5");
            ex = this.crypt(key.digest(this.email.getBytes())).trim() + "==";
            String addToUsers = "Insert Into Users(name,email,pass,hash) Values(\'" + this.name + "\',\'" + this.email + "\',\'" + pass + "\',\'" + ex + "\');";
            this.st.execute(addToUsers);
            this.hash = ex;
            this.createTables();
        } catch (SQLException var7) {
            if(var7.getMessage().contains("Duplicate entry")) {
                result = "<message>This email address have been registered</message>";
            }
        } catch (NoSuchAlgorithmException var8) {
            ;
        }

        return result;
    }

    private String logIn(String mac, String pass) {
        String result = "";

        try {
            ResultSet ex = this.st.executeQuery("Select COUNT(\'id\') cnt,hash,name From Users Where email=\'" + this.email + "\' AND pass=\'" + pass + "\';");
            ex.next();
            if(ex.getInt("cnt") != 1) {
                throw new NoSuchElementException();
            }

            this.hash = ex.getString("hash");
            this.name = ex.getString("name");
            this.checkIn(mac);
            result = result + "<name>" + this.name + "</name>";
            result = result + "<hash>" + this.hash + "</hash></xml>";
        } catch (SQLException var5) {
            result = result + "<message>Wrong request</message><email></xml>";
        } catch (NoSuchElementException var6) {
            result = result + "<message>The user have not been registered or login or password is incorrect</message></xml>";
        }

        return result;
    }

    private void checkIn(String mac) {
        try {
            this.st.execute("Insert Into `voplayer`.`" + this.hash + "_con`(MAC) Values(\'" + mac + "\');");
        } catch (SQLException var3) {
            ;
        }

    }

    private String logViaHash() {
        String result = "";

        try {
            ResultSet ex = this.st.executeQuery("Select Count(*) cnt,name From Users Where hash=\'" + this.hash + "\';");
            ex.next();
            if(ex.getInt("cnt") != 1) {
                throw new NoSuchElementException();
            }

            this.name = ex.getString("name");
            result = result + "<name>" + this.name + "</name>";
            result = result + "<hash>" + this.hash + "</hash></xml>";
        } catch (SQLException var3) {
            result = result + "<message>Wrong request</message><email></xml>";
        } catch (NoSuchElementException var4) {
            result = result + "<message>The user have not been registered</message></xml>";
        }

        return result;
    }

    private void logOut(String mac) {
        try {
            this.st.execute("Delete From `voplayer`.`" + this.hash + "_con` Where MAC= \'" + mac + "\';");
        } catch (SQLException var3) {
            var3.printStackTrace();
        }

    }

    private String write(String eng, String rur) {
        String res = "";

        try {
            this.st.execute("Insert Into `voplayer`.`" + this.hash + "_en`(eng,ru) Values(\'" + this.decrypt(eng) + "\',\'" + this.decrypt(rur) + "\');");
            res = res + this.decrypt(eng) + "\',\'" + this.decrypt(rur) + "</xml>";
        } catch (SQLException var5) {
            res = res + "</xml>";
        }

        return res;
    }

    private void createTables() throws SQLException {
        String connections = "CREATE  TABLE `voplayer`.`" + this.hash + "_con` (`id` INT NOT NULL AUTO_INCREMENT,`MAC` VARCHAR(45) NOT NULL," + "PRIMARY KEY (`id`),  UNIQUE INDEX `MAC_UNIQUE` (`MAC` ASC)) DEFAULT CHARACTER SET = utf8;";
        String engWord = "CREATE  TABLE `voplayer`.`" + this.hash + "_en` (`id` INT NOT NULL AUTO_INCREMENT ,`eng` VARCHAR(100) NOT NULL ," + "`ru` VARCHAR(100) NOT NULL ,PRIMARY KEY (`id`) ,UNIQUE INDEX `en_UNIQUE` (`eng` ASC) ) DEFAULT CHARACTER SET = utf8;";
        this.st.execute(connections);
        this.st.execute(engWord);
    }

    private void isSecure(String mac) throws SecurityException {
        try {
            ResultSet e = this.st.executeQuery("Select Count(*) cnt From Users Where hash=\'" + this.hash + "\';");
            e.next();
            if(e.getInt("cnt") != 1) {
                throw new SecurityException("Unknown user");
            }

            e = this.st.executeQuery("Select Count(*) cnt From `" + this.hash + "_con` Where MAC=\'" + mac + "\';");
            e.next();
            if(e.getInt("cnt") != 1) {
                throw new SecurityException("Unknown user");
            }
        } catch (SQLException var3) {
            var3.printStackTrace();
        }

    }

    private String info() {
        try {
            ResultSet e = this.st.executeQuery("Select Count(*) cnt From `" + this.hash + "_en`;");
            e.next();
            return "<info>" + e.getInt("cnt") + "</info></xml>";
        } catch (SQLException var2) {
            return "<message>Wrong request" + var2.getMessage() + "</message></xml>";
        }
    }

    private String read(int num) {
        try {
            String e = "";
            ResultSet qv = this.st.executeQuery("Select eng, ru From `" + this.hash + "_en`;");
            int i = 1;

            while(qv.next()) {
                if(num == i++) {
                    e = e + "<en>" + qv.getString("eng") + "</en>";
                    e = e + "<ru>" + qv.getString("ru") + "</ru>";
                    break;
                }
            }

            return e + "/xml>";
        } catch (SQLException var5) {
            return "<message>Wrong request</message></xml>";
        }
    }
}
 
