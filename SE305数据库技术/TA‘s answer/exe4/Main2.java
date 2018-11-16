import com.mysql.jdbc.jdbc2.optional.MysqlXAConnection;
import com.mysql.jdbc.jdbc2.optional.MysqlXid;

import javax.sql.XAConnection;
import javax.transaction.xa.XAException;
import javax.transaction.xa.XAResource;
import javax.transaction.xa.Xid;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;
import java.sql.ResultSet;

class ProblemSolutions
{
   
    static void problem1A(Connection conn1) throws SQLException
    {
        Statement stat = conn1.createStatement();

        String sql = "select avg(runtime) from movies where release_year = 1963";
        ResultSet rs = stat.executeQuery(sql);
        while (rs.next()) {
            System.out.println("1963年上映所有电影的平均放映时间: " + rs.getString("avg(runtime)"));
        }
        rs.close();
    }

    static void problem1B(Connection conn2) throws SQLException
    {
        Statement stat = conn2.createStatement();

        for (int i = 1000000; i < 1000000 + 1000; ++i) {
            String movie_id = "xx" + i;
            String title = "Title" + Math.random();
            int release_year = 1963;
            int runtime = (int) (Math.random() * 200);
            String tuple = "('" + movie_id + "','" + title + "'," + release_year + ',' + runtime + ')';
            stat.executeUpdate("insert into schema1.movies values " + tuple);
        }
    }

    static void problem2A(Connection conn1) throws SQLException
    {
        Statement stat = conn1.createStatement();

        String sql = "select avg(runtime) from movies natural join schema2.ratings where rating > 5.0;";
        ResultSet rs = stat.executeQuery(sql);
        while (rs.next()) {
            System.out.println("评分高于5.0的所有电影的平均放映时间: " + rs.getString("avg(runtime)"));
        }
        rs.close();
    }

    static void problem2B(Connection conn2) throws SQLException
    {
        Statement stat = conn2.createStatement();

        String sql = "delete from schema1.movies " +
                "where release_year in (2016,2017) " +
                "and " +
                "(select rating from schema2.ratings where schema2.ratings.movie_id = schema1.movies.movie_id) > 5.0;";
        stat.executeUpdate(sql);
    }

    static void problem3A(Connection conn1) throws SQLException
    {
        Statement stat = conn1.createStatement();

        String sql = "update schema2.ratings set rating = rating + 0.5 " +
                "where (select release_year from schema1.movies where schema2.ratings.movie_id = schema1.movies.movie_id) = 1919;";

        stat.executeUpdate(sql);
    }

    static void problem3B(Connection conn2) throws SQLException
    {
        Statement stat = conn2.createStatement();

        String sql = "select person_name from schema2.cast_members natural join schema2.ratings natural join schema1.people " +
                "where rating > 8.0;";
        ResultSet rs = stat.executeQuery(sql);
        while (rs.next()) {
            System.out.println("参演评分高于8.0分的演员的名字: " + rs.getString("person_name"));
        }
        rs.close();
    }

    static void problem4A(Connection conn) throws SQLException
    {
        Statement stat = conn.createStatement();
        Statement queryStat = conn.createStatement();
        ResultSet rs = queryStat.executeQuery("select movie_id from movies order by runtime desc limit 1");

        while (rs.next()) {
            String deleted_id = '\'' + rs.getString("movie_id") + '\'';
            System.out.println(deleted_id);
            String sql1 = "delete from movies where movie_id = " + deleted_id;
            String sql2 = "delete from schema2.ratings where movie_id = " + deleted_id;
            String sql3 = "delete from schema2.cast_members where movie_id = " + deleted_id;
            String sql4 = "delete from schema2.directors where movie_id = " + deleted_id;
            stat.executeUpdate(sql1);
            stat.executeUpdate(sql2);
            stat.executeUpdate(sql3);
            stat.executeUpdate(sql4);
        }
        rs.close();
    }

    static void problem4B(Connection conn) throws SQLException
    {
        Statement stat = conn.createStatement();

        String sql = "select title, release_year from schema1.movies order by release_year limit 10;";
        ResultSet rs = stat.executeQuery(sql);
        while (rs.next()) {
            System.out.println("最早十部电影的标题: "
                    + rs.getString("title")
                    + "; 上映年份: "
                    + rs.getString("release_year"));
        }
        rs.close();
    }


    static void problem5A(Connection conn) throws SQLException
    {
        Statement stat2 = conn.createStatement();


        String sql1 = "alter table people add show_time int;";
        String sql2 = "update people set show_time = " +
                "(select temp.counts from (" +
                "select person_id, count(movie_id) counts " +
                "from schema2.cast_members natural join people " +
                "group by person_id ) temp " +
                "where people.person_id = temp.person_id);";

        //conn.createStatement().executeUpdate("alter table people add show_time int;");
        stat2.executeUpdate(sql2);

    }

    static void problem5B(Connection conn) throws SQLException
    {
        Statement stat = conn.createStatement();
        String sql = "select person_name " +
                "from cast_members natural join schema1.movies natural join schema1.people " +
                "group by person_id " +
                "order by max(release_year) - min(release_year) desc limit 1";
        ResultSet rs = stat.executeQuery(sql);
        while (rs.next()) {
            System.out.println("职业生涯最长的演员名: "
                    + rs.getString("person_name"));
        }
        rs.close();
    }


}

public class Main2
{
    public static void main(String[] args) throws SQLException
    {
        //true represents print XA logs for debugging
        boolean logXaCommands = true;
        //Gain the instance of resource management(RM1)
        Connection conn1 = DriverManager.getConnection
                ("jdbc:mysql://localhost:3306/schema1?useSSL=false", "admin", "admin");
        XAConnection xaConn1 = new MysqlXAConnection((com.mysql.jdbc.Connection) conn1, logXaCommands);
        XAResource rm1 = xaConn1.getXAResource();
        // Gain the instance of resource management(RM2)
        Connection conn2 = DriverManager.getConnection
                ("jdbc:mysql://localhost:3306/schema2?useSSL=false", "admin", "admin");
        XAConnection xaConn2 = new MysqlXAConnection((com.mysql.jdbc.Connection) conn2, logXaCommands);
        XAResource rm2 = xaConn2.getXAResource();
        // Executing a distributed transaction using 2PC(2-phase-commit)
        try {
            // TM generates the transaction id on rm1
            Xid xid1 = new MysqlXid(new byte[]{0x01}, new byte[]{0x02}, 1);

            // For 5A
            //conn1.createStatement().executeUpdate("alter table people add show_time int;");

            // Executing the transaction on rm1
            rm1.start(xid1, XAResource.TMNOFLAGS);//One of TMNOFLAGS, TMJOIN, or TMRESUME.

            ProblemSolutions.problem1A(conn1);
            //ProblemSolutions.problem2A(conn1);
            //ProblemSolutions.problem3A(conn1);
            //ProblemSolutions.problem4A(conn1);
            //ProblemSolutions.problem5A(conn1);
            rm1.end(xid1, XAResource.TMSUCCESS);
            // TM generates the transaction id on rm1/
            Xid xid2 = new MysqlXid(new byte[]{0x01}, new byte[]{0x03}, 1);
            // Executing the transaction on rm1
            rm2.start(xid2, XAResource.TMNOFLAGS);

            ProblemSolutions.problem1B(conn2);
            //ProblemSolutions.problem2B(conn2);
            //ProblemSolutions.problem3B(conn2);
            //ProblemSolutions.problem4B(conn2);
            //ProblemSolutions.problem5B(conn2);
            rm2.end(xid2, XAResource.TMSUCCESS);
            // ===================Two Phase Commit================================
            // phase1：
            int ret1 = rm1.prepare(xid1);
            int ret2 = rm2.prepare(xid2);
            // phase2：
            if (XAResource.XA_OK == ret1 && XAResource.XA_OK == ret2) {
                rm1.commit(xid1, false);
                rm2.commit(xid2, false);
            } else {
                rm1.rollback(xid1);
                rm2.rollback(xid2);
            }

        } catch (XAException e) {
            e.printStackTrace();
        }
    }


}


