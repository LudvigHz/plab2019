
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressBar;
import javafx.scene.control.Slider;
import javafx.scene.paint.Color;
import plab.serial.PLabSerial;


public class MyController {
	
	private PLabSerial serialConnection = new PLabSerial();
	
	@FXML
	private Button defensiveModeButton;
	
	@FXML
	private Button offensiveModeButton;
	
	@FXML	
	private TextField serialPortTextField;
	
	@FXML
	void initialize() {		
		String portName;
		if(serialConnection.openPLabPort()) {	
		    portName = serialConnection.getOpenPortName();}
		else {
			portName = "No PLab BT device found.";
		};
		serialPortTextField.setText(portName);
		serialConnection.addListener(this, "messageRecieved");	
		
	}
	
	public void messageRecieved(String message, int value) {
		System.out.println(message);
	}
	@FXML
	void defensiveButtonPressed() {
	    serialConnection.sendMessage("#defensiveButtonPressed",0);
	}
	
	@FXML
	void offensiveButtonPressed() {
	    serialConnection.sendMessage("#offensiveButtonPressed",0);
	}
	
}
