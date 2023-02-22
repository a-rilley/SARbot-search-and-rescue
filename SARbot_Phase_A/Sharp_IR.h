// Map analog value to voltage measurement
float mapVoltage(int measuredVolt) { // Manual scaling to preserve float precision
  return float((measuredVolt * 3.3) / (1023.0)); // scale measured signal of 0-1023 to 0-5V
}

// Linear interpolation to determine distance from voltage
// Note: Only works for Sharp GP2Y0E02A (linear graph for voltage vs distance)
// Other sensors will require a different formula
float mapDistance(float measuredDist){
  return float(((0.55-measuredDist)*(4) + (measuredDist-2.21)*(50)) / (0.55-2.21));
}

void readIR() {
  inputVoltage = analogRead(sharpIR);
  scaleVolt = mapVoltage(inputVoltage);
  scaleDist = mapDistance(scaleVolt);

  delay(250);
}

// NOTE: Might use on/off logic to determine if rear is clear instead of above code
// Why: To log path type (i.e. T, cross, etc)
