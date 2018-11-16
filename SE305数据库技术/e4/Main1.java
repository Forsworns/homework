    		/*String sqlPhantom1_1 = "select mname from movies where myear=1200;";
    		String sqlPhantom1_1c = "commit;";
    		String sqlPhantom2_1 = "insert into movies value('tt1001','this is a test film',1200,600);";
    		String sqlPhantom1_2 = "select mname from movies where myear=1200;";
    		String sqlPhantom1_2c = "commit;";
    		String sqlPhantom2_1c = "commit;";
    		
    		
    		String sqlDirty1_1 = "select mname from movies where myear=1200;";
    		String sqlDirty1_1c = "commit;";
    		String sqlDirty2_1 = "update movies set mname='Dirty read test!' where myear=1200";
    		String sqlDirty1_2 = "select mname from movies where myear=1200;";
    		String sqlDirty1_2c = "commit;";
    		String sqlDirty2_1r = "rollback;";
    		String sqlDirty1_3 = "select mname from movies where myear=1200;";
    		String sqlDirty1_3c = "commit;";

    		
    		
    		String sqlRepeated1_1 = "select mname from movies where myear=1200;";
    		String sqlRepeated1_1c = "commit;";
    		String sqlRepeated2_1 = "update movies set mname='Repeated reading test!' where myear=1200";
    		String sqlRepeated2_1c = "commit;";
    		String sqlRepeated1_2 = "select mname from movies where myear=1200;";
    		String sqlRepeated1_2c = "commit;";*/
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

class Flag{
	boolean flag = false; //协调两个线程
	public void waitOnFlag(boolean direction) {
		synchronized(this) {
			if(direction) {
				if(flag) {
					try {
						wait();// 即this.wait()，object是this
					}catch(Exception e) {}
				}
			}else {
				if(!flag) {
					try {
						wait();
					}catch(Exception e) {}
				}
			}
		}
	}
	public void releaseFlag() {
		flag = !flag;
		synchronized(this) {
			this.notify();
		}
	}
}

class Thread1 extends Thread{  
	private Connection conn; 
	private Flag flag;
	public Thread1(Connection conn, Flag flag) { 
		this.conn=conn;
		this.flag = flag;
	    }  
    public void run() {  
    	try {
    		System.out.println("Thread1 running");
    		
    		//String sqlSetLevel = "set session transaction isolatin level read uncommitted";

    		Statement stat = conn.createStatement();
    		String sqlCheck = "select @@transaction_isolation";
			ResultSet rs =  stat.executeQuery(sqlCheck);
			while(rs.next()) {
				System.out.println("thread1 the level is "+rs.getString("@@transaction_isolation"));
			}
			rs.close();
			conn.commit();
			
			System.out.println("thread1 phantom");
			stat = conn.createStatement();
			String sqlPhantom1_1 = "select mname from movies where myear=1200;";
			rs =  stat.executeQuery(sqlPhantom1_1);
			boolean isEmpty = true;
			while(rs.next()) {
				isEmpty = false;
				System.out.println("thread1_p1 the name of movie is"+rs.getString("mname"));
			}
			if(isEmpty) System.out.println("thread1_p1 the name of movie is null");
			isEmpty = true;
			rs.close();
			conn.commit();
			flag.releaseFlag();
			
			flag.waitOnFlag(true);// t2 insert uncommit

			stat = conn.createStatement();
			String sqlPhantom1_2 = "select mname from movies where myear=1200;";
			rs =  stat.executeQuery(sqlPhantom1_2);
			while(rs.next()) {
				isEmpty = false;
				System.out.println("thread1_p2 the name of movie is"+rs.getString("mname"));
			}
			if(isEmpty) System.out.println("thread1_p1 the name of movie is null");
			isEmpty = true;
			rs.close();
			conn.commit();
			flag.releaseFlag();
			
			flag.waitOnFlag(true);// t2 commit
			
			System.out.println("thread1 dirty");
			stat = conn.createStatement();
			String sqlDirty1_1 = "select mname from movies where myear=1200;";
			rs =  stat.executeQuery(sqlDirty1_1);
			while(rs.next()) {
				System.out.println("thread1_d1 the name of movie is"+rs.getString("mname"));
				isEmpty = false;
			}
			if(isEmpty) System.out.println("thread1_p1 the name of movie is null");
			isEmpty = true;
			rs.close();
			conn.commit();
			flag.releaseFlag();
			
			flag.waitOnFlag(true);//update
			
			stat = conn.createStatement();
			String sqlDirty1_2 = "select mname from movies where myear=1200;";
			rs =  stat.executeQuery(sqlDirty1_2);
			while(rs.next()) {
				isEmpty = false;
				System.out.println("thread1_d2 the name of movie is"+rs.getString("mname"));
			}
			if(isEmpty) System.out.println("thread1_p1 the name of movie is null");
			isEmpty = true;
			rs.close();
			conn.commit();
			flag.releaseFlag();
			
			flag.waitOnFlag(true);//rollback
			
			stat = conn.createStatement();
			String sqlDirty1_3 = "select mname from movies where myear=1200;";
			rs =  stat.executeQuery(sqlDirty1_3);
			while(rs.next()) {
				isEmpty = false;
				System.out.println("thread1_d3 the name of movie is"+rs.getString("mname"));
			}
			if(isEmpty) System.out.println("thread1_p1 the name of movie is null");
			isEmpty = true;
			rs.close();
			conn.commit();

			System.out.println("thread1 repeat");
			stat = conn.createStatement();
			String sqlRepeated1_1 = "select mname from movies where myear=1200;";
			rs =  stat.executeQuery(sqlRepeated1_1);
			while(rs.next()) {
				isEmpty = false;
				System.out.println("thread1_r1 the name of movie is"+rs.getString("mname"));
			}
			if(isEmpty) System.out.println("thread1_p1 the name of movie is null");
			isEmpty = true;
			rs.close();
			flag.releaseFlag();
    		
			flag.waitOnFlag(true);//update commit
    		
			stat = conn.createStatement();
			String sqlRepeated1_2 = "select mname from movies where myear=1200;";
			rs =  stat.executeQuery(sqlRepeated1_2);
			while(rs.next()) {
				isEmpty = false;
				System.out.println("thread1_r2 the name of movie is"+rs.getString("mname"));
			}
			if(isEmpty) System.out.println("thread1_p1 the name of movie is null");
			rs.close();
			conn.commit();
			flag.releaseFlag();
    		
			System.out.println("Thread1 exits");  
    	 }
    	 catch (SQLException ex) {
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

class Thread2 extends Thread{  
    private Connection conn; 
	private Flag flag;
    public Thread2(Connection conn,Flag flag) {  
       this.conn=conn; 
       this.flag = flag; 
    }  
    public void run() { 
        try {

    		System.out.println("Thread2 running"); 	
    		Statement stat;
    		System.out.println("thread2 phantom");
    		
    		flag.waitOnFlag(false);// select
    		
    		stat = conn.createStatement();
    		String sqlPhantom2_1 = "insert into movies value('tt1001','this is a test film',1200,600);";
			stat.executeUpdate(sqlPhantom2_1);
			System.out.println("thread2 insert before commit");
			flag.releaseFlag();
    		
			flag.waitOnFlag(false);// select
			
			conn.commit();
			System.out.println("thread2 insert commit");
			flag.releaseFlag();
    		
			
    		System.out.println("thread2 dirty ");
    		flag.waitOnFlag(false);// select
    		
    		stat = conn.createStatement();
    		String sqlDirty2_1 = "update movies set mname='Dirty read test!' where myear=1200";
			stat.executeUpdate(sqlDirty2_1);
			System.out.println("thread2 update before rollback");
			flag.releaseFlag();
			
			flag.waitOnFlag(false);// select

			conn.rollback();
			System.out.println("thread2 update rollback");
			flag.releaseFlag();
			
			flag.waitOnFlag(false);// select
    		
    		System.out.println("thread2 repeat");
    		flag.waitOnFlag(false);// select
    		
    		stat = conn.createStatement();
    		String sqlRepeated2_1 = "update movies set mname='Repeated reading test!' where myear=1200";
			stat.executeUpdate(sqlRepeated2_1);
    		conn.commit();
			System.out.println("thread2 update commit");
			flag.releaseFlag();
    		
			flag.waitOnFlag(false);// select
    		
    		System.out.println("Thread2 exits");  
    	 }
	     catch (SQLException ex) {
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

public class Main { 
    public static void main(String[] args) throws ClassNotFoundException {  
    	try { 
    		Connection conn1 = DriverManager.getConnection("jdbc:mysql://localhost:3306/mydb?useSSL=false", "admin","admin");
    		Connection conn2 = DriverManager.getConnection("jdbc:mysql://localhost:3306/mydb?useSSL=false", "admin","admin");
			conn1.setAutoCommit(false);
			conn2.setAutoCommit(false);
			Flag flag = new Flag();
    		Thread1 mTh1=new Thread1(conn1,flag);  
    		Thread2 mTh2=new Thread2(conn2,flag);  
    		mTh1.start();  
    		mTh2.start();  
    	}
    	 catch( Exception e )  
        {  
            e.printStackTrace();  
        }  
    }  

}  

