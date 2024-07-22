package com.drthoms;

import com.drthoms.database.Taffel;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author drthoms
 */
public class JDBCConnection {

    private static final String DB_CONNECTION = "jdbc:mysql://127.0.0.1:3306/cheesydb";
    private static final String ROOT = "root";
    private static final String PASSWORD = "2Super0Simple2Password4";

    public Taffel getTaffel(String id) {
        String sel = "SELECT * FROM taffel WHERE id=" + id;
        try ( Connection connect = setupConnection()) {
            Statement statement = connect.createStatement();
            ResultSet resultSet = statement.executeQuery(sel);
            Taffel table = new Taffel();
            while (resultSet.next()) {
                table.setId(resultSet.getLong("ID"));
                table.setLine(resultSet.getString("LINE"));
            }
            return table;
        } catch (SQLException err) {
            System.err.format("SQL State: %s\n%s", err.getSQLState(), err.getMessage());
        }
        return null;
    }

    public List<Taffel> getTaffels() {
        List<Taffel> taffels = new ArrayList<>();
        String sel = "SELECT * FROM taffel";
        try ( Connection connect = setupConnection()) {
            Statement statement = connect.createStatement();
            ResultSet resultSet = statement.executeQuery(sel);
            while (resultSet.next()) {
                Taffel table = new Taffel();
                table.setId(resultSet.getLong("ID"));
                table.setLine(resultSet.getString("LINE"));
                taffels.add(table);
            }
        } catch (SQLException err) {
            System.err.format("SQL State: %s\n%s", err.getSQLState(), err.getMessage());
        }
        return taffels;
    }

    public String addToTable(String line) {
        String insert = "INSERT INTO taffel (line) VALUES ('" + line + "')";
        try ( Connection connect = setupConnection()) {
            Statement statement = connect.createStatement();
            statement.execute(insert);
        } catch (SQLException err) {
            System.err.format("SQL State: %s\n%s", err.getSQLState(), err.getMessage());
            return "Post Failed\n";
        }
        return "Post Successful\n";
    }

    public String deleteFromTable(String id) {
        String del = "DELETE FROM taffel WHERE id=" + id;
        try ( Connection connect = setupConnection()) {
            Statement statement = connect.createStatement();
            statement.execute(del);
        } catch (SQLException err) {
            System.err.format("SQL State: %s\n%s", err.getSQLState(), err.getMessage());
            return "Delete Failed\n";
        }
        return "Delete Successful\n";
    }

    private Connection setupConnection() throws SQLException {
        return DriverManager.getConnection(DB_CONNECTION, ROOT, PASSWORD);
    }
}

