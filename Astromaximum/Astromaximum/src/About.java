/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author willow
 */
import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.lcdui.*;
/**
 * This alert shows some info about application and offers to go
 * to the website (if platform allows <b>System.platformRequest</b>).
 * @author willow
 */
public class About extends Alert implements CommandListener{
    About(){
        super(Astromaximum.getstr(152),
                Astromaximum.getstr(153)+" "+Astromaximum.URL+" "+
                Astromaximum.getstr(154), null, AlertType.INFO);
        addCommand(new Command(Astromaximum.getstr(94), Command.BACK, 0));
        addCommand(new Command(Astromaximum.getstr(155), Command.SCREEN, 0));
        setCommandListener(this);
    }
    
    public void commandAction(Command c, Displayable d){

        if(c.getCommandType()==Command.BACK){
            Astromaximum.disp.setCurrent(Astromaximum.summary);
        }
        else{
            try {
                Astromaximum.instance.platformRequest(Astromaximum.URL);
            } catch (ConnectionNotFoundException ex) {
                ex.printStackTrace();
            }
        }
    }
}
