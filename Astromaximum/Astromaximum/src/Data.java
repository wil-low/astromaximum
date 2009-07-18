/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author willow
 */
final class Data {
    static private final String common = "COMMON_DAT";
    static private final String locations[] = {"LOCATIONS_DAT0", "LOCATIONS_DAT1", "LOCATIONS_DAT2", "LOCATIONS_DAT3"};

    byte[] getCommon ()
    {
        return common.getBytes();
    }
    byte[] getLocations ()
    {
        int count = 0;
        for (int i = 0; i < locations.length; ++i) {
            count += locations[i].length();
        }
        byte[] arr = new byte[count];
        count = 0;
        for (int i = 0; i < locations.length; ++i) {
            int len = locations[i].length();
            System.arraycopy(locations[i].getBytes(), 0, arr, count, len);
            count += len;
        }
        return arr;
    }
}
