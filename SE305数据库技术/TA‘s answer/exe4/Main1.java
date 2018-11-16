import java.sql.SQLException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;
import java.sql.ResultSet;

class ProblemSolutions
{

    static void problem1A(Connection conn) throws SQLException
    {
        Statement stat = conn.createStatement();

        String sql = "select avg(runtime) from movies where release_year = 1963";
        ResultSet rs = stat.executeQuery(sql);
        while (rs.next()) {
            System.out.println("1963年上映所有电影的平均放映时间: " + rs.getString("avg(runtime)"));
        }
        rs.close();
    }

    static void problem1B(Connection conn) throws SQLException
    {
        Statement stat = conn.createStatement();

        for (int i = 1000000; i < 1000000 + 1000; ++i) {
            String movie_id = "xx" + i;
            String title = "Title" + Math.random();
            int release_year = 1963;
            int runtime = (int) (Math.random() * 200);
            String tuple = "('" + movie_id + "','" + title + "'," + release_year + ',' + runtime + ')';
            stat.executeUpdate("insert into movies values " + tuple);
        }
    }

    static void problem2A(Connection conn) throws SQLException
    {
        Statement stat = conn.createStatement();

        String sql = "select avg(runtime) from movies natural join ratings where rating > 5.0;";
        ResultSet rs = stat.executeQuery(sql);
        while (rs.next()) {
            System.out.println("评分高于5.0的所有电影的平均放映时间: " + rs.getString("avg(runtime)"));
        }
        rs.close();
    }

    static void problem2B(Connection conn) throws SQLException
    {
        Statement stat = conn.createStatement();

        String sql = "delete from movies " +
                "where release_year in (2016,2017) " +
                "and " +
                "(select rating from ratings where ratings.movie_id = movies.movie_id) > 5.0;";
        stat.executeUpdate(sql);
    }

    static void problem3A(Connection conn) throws SQLException
    {
        Statement stat = conn.createStatement();

        String sql = "update ratings set rating = rating + 0.5 " +
                "where (select release_year from movies where ratings.movie_id = movies.movie_id) = 1919;";

        stat.executeUpdate(sql);
    }

    static void problem3B(Connection conn) throws SQLException
    {
        Statement stat = conn.createStatement();

        String sql = "select person_name from cast_members natural join ratings natural join people " +
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
            String sql2 = "delete from ratings where movie_id = " + deleted_id;
            String sql3 = "delete from cast_members where movie_id = " + deleted_id;
            String sql4 = "delete from directors where movie_id = " + deleted_id;
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

        String sql = "select title, release_year from movies order by release_year limit 10;";
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
        Statement stat = conn.createStatement();

        String sql1 = "alter table people add show_time int;";
        String sql2 = "update people set show_time = " +
                "(select temp.counts from (" +
                "select person_id, count(movie_id) counts " +
                "from cast_members natural join people " +
                "group by person_id ) temp " +
                "where people.person_id = temp.person_id);";
        stat.executeUpdate(sql1);
        stat.executeUpdate(sql2);

    }

    static void problem5B(Connection conn) throws SQLException
    {
        Statement stat = conn.createStatement();
        String sql = "select person_name " +
                "from cast_members natural join movies natural join people " +
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





class Thread1 extends Thread
{
    private Connection conn;

    Thread1(Connection conn)
    {
        this.conn = conn;
    }

    public void run()
    {
        try {
            System.out.println("Thread1 running");
            ProblemSolutions.problem1A(conn);
            //ProblemSolutions.problem2A(conn);
            //ProblemSolutions.problem3A(conn);
            //ProblemSolutions.problem4A(conn);
            //ProblemSolutions.problem5A(conn);
            conn.commit();
        } catch (SQLException ex) {
            ex.printStackTrace();
            try {
                //An error occured so we rollback the changes.
                this.conn.rollback();
            } catch (SQLException ex1) {
                ex1.printStackTrace();
            }
        }
    }
}

class Thread2 extends Thread
{
    private Connection conn;

    Thread2(Connection conn)
    {
        this.conn = conn;
    }

    public void run()
    {
        try {
            System.out.println("Thread2 running");
    
            ProblemSolutions.problem1B(conn);
            //ProblemSolutions.problem2B(conn);
            //ProblemSolutions.problem3B(conn);
            //ProblemSolutions.problem4B(conn);
            //ProblemSolutions.problem5B(conn);
            conn.commit();
        } catch (SQLException ex) {
            ex.printStackTrace();
            try {
                //An error occured so we rollback the changes.
                this.conn.rollback();
            } catch (SQLException ex1) {
                ex1.printStackTrace();
            }
        }
    }

}

public class Main1
{
    public static void main(String[] args) throws ClassNotFoundException
    {
        try {
            Connection conn1 = DriverManager.getConnection("jdbc:mysql://localhost:3306/sys?useSSL=false", "admin", "admin");
            Connection conn2 = DriverManager.getConnection("jdbc:mysql://localhost:3306/sys?useSSL=false", "admin", "admin");
            conn1.setAutoCommit(false);
            conn2.setAutoCommit(false);
            Thread1 mTh1 = new Thread1(conn1);
            Thread2 mTh2 = new Thread2(conn2);
            mTh1.start();
            mTh2.start();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}  


