# Technical Requirement

**Technical Requirement**:  
The DERT device shall report environmental variables via UART interface.

**Rationale**:  
UART is the most commonly supported serial interface. It's maximum data rate of 5 Mbps is more than adequate for low-frequency environmental variables. It is supported by the user's upstream hardware.

Previous iteration: #7

## Verification Plan

**Test Parameter**: UART Serial Connection  
**Test Parameter Unit**: not applicable

**Realizable and Ideal Verification Process**:  
The DERT device shall be connected to a UART-enabled computer through it's serial port. The requirement will be met if data is successfully shown to be transmitted from the DERT device and received by the computer.

**Verification Equipment**:  
- DERT device
- UART to USB cable
- Computer equipped with COM-capable USB port

**Verification Evidence**:
The file "[2022-05-29-uart-verf.txt](2022-05-29-uart-verf.txt)" demonstrates successful output over UART. All sensors were disconnected at the time of the test, which is why there are errors reported. However, the text shows two successful startup sequences and several task executions, demonstrating that the DERT device can communicate over the same UART channel across multiple power cycles.

**Verification Result**:  
0 - Verification process was not completed.  
1 - Requirement was NOT met.  
2 - Requirement was NOT met, but the verification process was not ideal so the result may not be accurate or complete.  
3 - Requirement *was* met, but the verification process was not ideal so the result may not be accurate or complete.  
4 - Requirement *was* met.  
X - Requirement was not approved, so it could not be verified.