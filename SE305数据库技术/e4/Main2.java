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
import java.util.ArrayList;
import java.util.Set;
import java.sql.ResultSet;
 

public class Main {
   private static class IdTime{
	   int time;
   	   String id;
   	   IdTime(String s,int i){
   		   id = s;
   		   time = i;
   	   }
   };
   
   public static void main(String[] args) throws SQLException {
      //true represents print XA logs for debugging
      boolean logXaCommands = true;
      //Gain the instance of resource management(RM1)
      Connection conn1 = DriverManager.getConnection("jdbc:mysql://localhost:3306/xa1?useSSL=false", "admin", "admin");
      XAConnection xaConn1 = new MysqlXAConnection((com.mysql.jdbc.Connection) conn1, logXaCommands);
      XAResource rm1 = xaConn1.getXAResource();
    // Gain the instance of resource management(RM2)
      Connection conn2 = DriverManager.getConnection("jdbc:mysql://localhost:3306/xa2?useSSL=false", "admin","admin");
      XAConnection xaConn2 = new MysqlXAConnection((com.mysql.jdbc.Connection) conn2, logXaCommands);
      XAResource rm2 = xaConn2.getXAResource();
      
      // Executing a distributed transaction using 2PC(2-phase-commit)
      try {
          // TM generates the transaction id on rm1
     	 byte[] gtrid1 = "trans1".getBytes();
     	 byte[] bqual1 = "b1".getBytes(); 
          Xid xid1 = new MysqlXid(gtrid1,bqual1,1);
          byte[] gtrid2 = "trans2".getBytes();
     	 byte[] bqual2 = "b2".getBytes(); 
          Xid xid2 = new MysqlXid(gtrid2,bqual2,1);
    	 
          // 1A
          String sql1A = "select avg(mtime)\r\n" + 
           		"from xa1.movies\r\n" + 
           		"where myear=1963 and mtime!=null;";
         // Executing the transaction on rm1
         rm1.start(xid1, XAResource.TMNOFLAGS);//One of TMNOFLAGS, TMJOIN, or TMRESUME.
         PreparedStatement ps1 = conn1.prepareStatement(sql1A);
         ResultSet rs = ps1.executeQuery();
         while(rs.next()) {
        	 System.out.println("thread1 the avg time of movies in 1963 is"+rs.getString("avg(mtime)"));
         }
         rs.close();
         rm1.end(xid1, XAResource.TMSUCCESS);
         

         rm2.start(xid2, XAResource.TMNOFLAGS);//One of TMNOFLAGS, TMJOIN, or TMRESUME.
         PreparedStatement ps2;
         for(int i=0;i<1000;++i) {
				String sql1B = "insert into xa1.movies values\r\n" + 
					"('tt9017"+String.valueOf(i)+"','The Big Adventure',1963,22);";
		         ps2 = conn2.prepareStatement(sql1B);
		         ps2.executeUpdate();
		 }
         rm2.end(xid2, XAResource.TMSUCCESS);
         // phase1：
         int rm1_prepare = rm1.prepare(xid1);
         int rm2_prepare = rm2.prepare(xid2);
         // phase2：  
         boolean onePhase = false;
         if(rm1_prepare==XAResource.XA_OK&&rm2_prepare==XAResource.XA_OK) {
        	 rm1.commit(xid1, onePhase);
        	 rm2.commit(xid2, onePhase);
        	 System.out.println("1A 查询成功");
        	 System.out.println("1B 插入成功");
         } else {
        	 rm1.rollback(xid1);
        	 rm2.rollback(xid2);
         }
         
         
         // 2A 沙雕做法
         rm1.start(xid1, XAResource.TMNOFLAGS);//One of TMNOFLAGS, TMJOIN, or TMRESUME.
         ArrayList<IdTime> idTimeList = new ArrayList<>();
         String sql2A1 = "select id,mtime from movies;";
		 ps1 = conn1.prepareStatement(sql2A1);
		 rs = ps1.executeQuery();
		 while(rs.next()) {
			 IdTime idTime = new IdTime(rs.getString("id"),rs.getInt("mtime"));
			 idTimeList.add(idTime);
		 }
		 rs.close();
		 System.out.println("2A 电影时间获取成功"+idTimeList.size());
         rm1.end(xid1, XAResource.TMSUCCESS);
         
          // Executing the transaction on rm2
         rm2.start(xid2, XAResource.TMNOFLAGS);
         ArrayList<String> scoreList = new ArrayList<>();
         String sql2A2 = "select id from ratings where mscore>5.0";
         ps2 = conn2.prepareStatement(sql2A2);
         rs = ps2.executeQuery();
         while(rs.next()) {
        	 scoreList.add(rs.getString("id"));
         }
         rs.close();
         System.out.println("2A 评分获取成功"+scoreList.size());
         rm2.end(xid2, XAResource.TMSUCCESS);
         double averageTime = 0;
         int movieNumber = 0;
         for(int i=0;i<idTimeList.size();++i) {
        	 if(scoreList.contains(idTimeList.get(i).id)) {
        		 averageTime+=idTimeList.get(i).time;
        		 movieNumber++;
        	 }
         }
         averageTime/=movieNumber;
         System.out.println("2A The avg time of movies with score greater than 5.0 is"+String.valueOf(averageTime));
         // ===================Two Phase Commit================================
         // phase1：
         rm1_prepare = rm1.prepare(xid1);
         rm2_prepare = rm2.prepare(xid2);
         // phase2：  
         onePhase = false;
         if(rm1_prepare==XAResource.XA_OK&&rm2_prepare==XAResource.XA_OK) {
        	 rm1.commit(xid1, onePhase);
        	 rm2.commit(xid2, onePhase);
        	 System.out.println("2A 查询成功");
         } else {
        	 rm1.rollback(xid1);
        	 rm2.rollback(xid2);
         }
         
         // 2B
         String sql2B = "delete from xa1.movies\r\n" + 
          		"where (myear=2016 or myear=2017) and \r\n" + 
          		"id in (\r\n" + 
          		"	select id\r\n" + 
          		"	from xa2.ratings\r\n" + 
          		"	where mscore>5.0\r\n" + 
          		");";
         // Executing the transaction on rm2
         rm2.start(xid2, XAResource.TMNOFLAGS);
         ps2 = conn2.prepareStatement(sql2B);
         ps2.execute();
         rm2.end(xid2, XAResource.TMSUCCESS);
         // ===================Two Phase Commit================================
         // phase1：
         rm2_prepare = rm2.prepare(xid2);
         // phase2：  
         onePhase = false;
         if(rm2_prepare==XAResource.XA_OK) {
        	 rm2.commit(xid2, onePhase);
    		 System.out.println("2B 删除成功");
         } else {
        	 rm2.rollback(xid2);
         }
         
         // 3A 
         String sql3A = "update xa2.ratings\r\n" + 
         		"set mscore = mscore +0.5\r\n" + 
         		"where id in (\r\n" + 
         		"	select id\r\n" + 
         		"    from xa1.movies\r\n" + 
         		"    where myear=1919\r\n" + 
         		");";
          // Executing the transaction on rm1
          rm1.start(xid1, XAResource.TMNOFLAGS);//One of TMNOFLAGS, TMJOIN, or TMRESUME.
 		 ps1 = conn1.prepareStatement(sql3A);
 		 ps1.execute();
          rm1.end(xid1, XAResource.TMSUCCESS);
          

          String sql3B = "select pname\r\n" + 
          		"from xa1.people\r\n" + 
          		"where pid in (\r\n" + 
          		"	select pid\r\n" + 
          		"    from xa2.cast_members\r\n" + 
          		"    where id in (\r\n" + 
          		"		select id\r\n" + 
          		"        from xa2.ratings\r\n" + 
          		"        where mscore>8.0\r\n" + 
          		"    )\r\n" + 
          		");";
          // Executing the transaction on rm2
          rm2.start(xid2, XAResource.TMNOFLAGS);
    		 ps2 = conn2.prepareStatement(sql3B);
       		 rs = ps2.executeQuery();
       		 while(rs.next()) {System.out.println("分数大于8.0电影的演员"+rs.getString("pname"));}
          rm2.end(xid2, XAResource.TMSUCCESS);
          // ===================Two Phase Commit================================
          // phase1：
          rm1_prepare = rm1.prepare(xid1);
          rm2_prepare = rm2.prepare(xid2);
          // phase2：  
          onePhase = false;
          if(rm1_prepare==XAResource.XA_OK&&rm2_prepare==XAResource.XA_OK) {
         	 rm1.commit(xid1, onePhase);
         	 rm2.commit(xid2, onePhase);
     		 System.out.println("3A更新成功");
     		System.out.println("3B查询成功");
          } else {
         	 rm1.rollback(xid1);
         	 rm2.rollback(xid2);
          }
          
          // 4A
            String sql4A1 = "delete from xa2.ratings\r\n" + 
            		"where id in (\r\n" + 
            		"	select id\r\n" + 
            		"    from xa1.movies\r\n" + 
            		"    where mtime=(\r\n" + 
            		"		select max(mtime)\r\n" + 
            		"        from xa1.movies\r\n" + 
            		"    )\r\n" + 
            		");";
            String sql4A2 = "delete from xa2.cast_members\r\n" + 
            		"where id in (\r\n" + 
            		"	select id\r\n" + 
            		"    from xa1.movies\r\n" + 
            		"    where mtime=(\r\n" + 
            		"		select max(mtime)\r\n" + 
            		"        from xa1.movies\r\n" + 
            		"    )\r\n" + 
            		");";
            String sql4A3 =	"delete from xa2.directors\r\n" + 
            		"where id in (\r\n" + 
            		"	select id\r\n" + 
            		"    from xa1.movies\r\n" + 
            		"    where mtime=(\r\n" + 
            		"		select max(mtime)\r\n" + 
            		"        from xa1.movies\r\n" + 
            		"    )\r\n" + 
            		");";
            String sql4A4 = "delete from xa1.movies\r\n" + 
            		"where mtime=(\r\n" + 
            		"	select max(mtime)\r\n" + 
            		");";
            
              // Executing the transaction on rm1
              rm1.start(xid1, XAResource.TMNOFLAGS);//One of TMNOFLAGS, TMJOIN, or TMRESUME.
     		 ps1 = conn1.prepareStatement(sql4A1);
     		 ps1.execute();
     		ps1 = conn1.prepareStatement(sql4A2);
    		 ps1.execute();
    		 ps1 = conn1.prepareStatement(sql4A3);
     		 ps1.execute();
     		ps1 = conn1.prepareStatement(sql4A4);
    		 ps1.execute();
              rm1.end(xid1, XAResource.TMSUCCESS);


              String sql4B = "select mname,myear\r\n" + 
              		"from xa1.movies\r\n" + 
              		"order by myear asc\r\n" + 
              		"limit 0,10;";
              // Executing the transaction on rm2，
              rm2.start(xid2, XAResource.TMNOFLAGS);
        		 ps2 = conn2.prepareStatement(sql4B);
        		 rs = ps2.executeQuery();
              while(rs.next()) {System.out.println("按照年份排列："+rs.getInt("myear")+rs.getString("mname"));}   
              rm2.end(xid2, XAResource.TMSUCCESS);
              // ===================Two Phase Commit================================
              // phase1：
              rm1_prepare = rm1.prepare(xid1);
              rm2_prepare = rm2.prepare(xid2);
              // phase2：  
              onePhase = false;
              if(rm1_prepare==XAResource.XA_OK&&rm2_prepare==XAResource.XA_OK) {
             	 rm1.commit(xid1, onePhase);
             	 rm2.commit(xid2, onePhase);
         		 System.out.println("4A删除成功");
         		System.out.println("4B查询成功");
              } else {
             	 rm1.rollback(xid1);
             	 rm2.rollback(xid2);
              }
                
         
          // 5A
                String sql5A1 = "alter table xa1.people\r\n" + 
                		"add column ptimes int;";
                String sql5A2 =	"update xa1.people \r\n" + 
                		"set ptimes = (\r\n" + 
                		"	select t.counts from(\r\n" + 
                		"		SELECT pid,count(id) counts\r\n" + 
                		"		from xa2.cast_members\r\n" + 
                		"		group by pid) as t\r\n" + 
                		"	where people.pid = t.pid\r\n" + 
                		");";
                  // Executing the transaction on rm1

        		 ps1 = conn1.prepareStatement(sql5A1);
        		 ps1.execute();//不可以在启动rm后的active状态下执行alter
                  rm1.start(xid1, XAResource.TMNOFLAGS);//One of TMNOFLAGS, TMJOIN, or TMRESUME.
         		 ps1 = conn1.prepareStatement(sql5A2);
         		 ps1.execute();
                  rm1.end(xid1, XAResource.TMSUCCESS);
                  
                  String sql5B1 = "insert into xa1.people(pid)\r\n" + 
                    		"	select distinct pid\r\n" + 
                    		"    from xa2.cast_members\r\n" + 
                    		"    where pid not in(\r\n" + 
                    		"		select pid\r\n" + 
                    		"		from xa1.people\r\n" + 
                    		"    );";
                    String sql5B2 = "select pname\r\n" + 
                    		"from xa1.people\r\n" + 
                    		"where pid in (\r\n" + 
                    		"	select t.pid\r\n" + 
                    		"	from(\r\n" + 
                    		"		select max(movies.myear)-min(movies.myear) as pro_life,cast_members.pid as pid\r\n" + 
                    		"		from xa1.movies,xa2.cast_members\r\n" + 
                    		"		where xa1.movies.id=xa2.cast_members.id\r\n" + 
                    		"		group by xa2.cast_members.pid\r\n" + 
                    		"    ) as t\r\n" + 
                    		"	where t.pro_life = (\r\n" + 
                    		"		select max(r.pro_life)\r\n" + 
                    		"        from (\r\n" + 
                    		"			select max(movies.myear)-min(movies.myear) as pro_life,cast_members.pid as pid\r\n" + 
                    		"			from xa1.movies,xa2.cast_members\r\n" + 
                    		"			where xa1.movies.id=xa2.cast_members.id\r\n" + 
                    		"			group by xa2.cast_members.pid\r\n" + 
                    		"        ) as r\r\n" + 
                    		"    )\r\n" + 
                    		");";
                  // Executing the transaction on rm2
                  rm2.start(xid2, XAResource.TMNOFLAGS);
	           		ps2 = conn2.prepareStatement(sql5B2);
	           		 rs = ps2.executeQuery();
	           		 while(rs.next()) {System.out.println("职业生涯最长的演员"+rs.getString("pname"));}
                  rm2.end(xid2, XAResource.TMSUCCESS);
                  // ===================Two Phase Commit================================
                  // phase1：
                  rm1_prepare = rm1.prepare(xid1);
                  rm2_prepare = rm2.prepare(xid2);
                  // phase2：  
                  onePhase = false;
                  if(rm1_prepare==XAResource.XA_OK&&rm2_prepare==XAResource.XA_OK) {
                 	 rm1.commit(xid1, onePhase);
                 	 rm2.commit(xid2, onePhase);
             		 System.out.println("5A添加/更新成功");
             		 System.out.println("5B查询成功");
                  } else {
                 	 rm1.rollback(xid1);
                 	 rm2.rollback(xid2);
                  }
                    
      } catch (XAException e) {
         e.printStackTrace();
      }
   }
}


