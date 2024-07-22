package com.drthoms;

import com.google.gson.Gson;
import com.drthoms.database.Taffel;
import fi.iki.elonen.NanoHTTPD;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;

import static fi.iki.elonen.NanoHTTPD.MIME_PLAINTEXT;
import static fi.iki.elonen.NanoHTTPD.newFixedLengthResponse;

/**
 *
 * @author drthoms
 */
public final class CurlUtils {

    private static final String NO_RESOURCE = "The requested resource does not exist";

    private CurlUtils() {
    }

    public static NanoHTTPD.Response performGet(JDBCConnection connection, NanoHTTPD.IHTTPSession session) {
        String jsonResp = null;
        String id = getIndex(session.getUri());
        Gson gson = new Gson();

        if (id != null && !id.equals("")) {
            Taffel table = connection.getTaffel(id);
            if (table == null) {
                return failedAttempt();
            }
            jsonResp = gson.toJson(table);
        } else {
            List<Taffel> tables = connection.getTaffels();
            if (tables.isEmpty()) {
                return failedAttempt();
            }
            jsonResp = gson.toJson(tables);
        }
        return newFixedLengthResponse(jsonResp);
    }

    public static NanoHTTPD.Response performPost(JDBCConnection connection, NanoHTTPD.IHTTPSession session) {
        try {
            session.parseBody(new HashMap<>());
            String res = connection.addToTable(session.getQueryParameterString());
            return newFixedLengthResponse(res);
        } catch (IOException | NanoHTTPD.ResponseException err) {
            return failedAttempt();
        }
    }

    public static NanoHTTPD.Response performDelete(JDBCConnection connection, NanoHTTPD.IHTTPSession session) {
        String result = connection.deleteFromTable(getIndex(session.getUri()));
        return newFixedLengthResponse(result);
    }

    public static NanoHTTPD.Response failedAttempt() {
        return newFixedLengthResponse(NanoHTTPD.Response.Status.NOT_FOUND, MIME_PLAINTEXT, NO_RESOURCE);
    }

    private static String getIndex(String param) {
        return param.replaceAll("[^0-9]", "");
    }
}

