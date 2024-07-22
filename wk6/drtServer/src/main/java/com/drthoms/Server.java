package com.drthoms;

import fi.iki.elonen.NanoHTTPD;
import fi.iki.elonen.NanoHTTPD.IHTTPSession;
import fi.iki.elonen.NanoHTTPD.Response;
import java.io.IOException;

/**
 *
 * @author drthoms
 */
public class Server extends NanoHTTPD {

    private JDBCConnection jdbcObj;

    public Server() throws IOException {
        super(8080);
        jdbcObj = new JDBCConnection();
        start(NanoHTTPD.SOCKET_READ_TIMEOUT, false);
    }

    public static void main(String[] args) {
        try {
            new Server();
        } catch (IOException err) {
            System.err.println("Couldn't start server:\n" + err);
        }
    }

    @Override
    public Response serve(IHTTPSession session) {
        if (session.getMethod() == Method.GET) {
            return CurlUtils.performGet(jdbcObj, session);
        } else if (session.getMethod() == Method.POST) {
            return CurlUtils.performPost(jdbcObj, session);
        } else if (session.getMethod() == Method.PUT) {
            return CurlUtils.performPost(jdbcObj, session);
        } else if (session.getMethod() == Method.DELETE) {
            return CurlUtils.performDelete(jdbcObj, session);
        }

        return CurlUtils.failedAttempt();
    }
}
