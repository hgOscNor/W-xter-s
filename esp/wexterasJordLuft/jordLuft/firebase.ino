
void firebase() {
  /*
    Set String, Int, Float, or Bool in Firebase
    
    Parameters:
      - path: The path in Firebase where the data will be stored.
      - data: The value to set, which can be of type String, Int, Float, or Bool.

    Returns:
      - HTTP response code as an integer.
        - 200 indicates success.
        - Other codes indicate failure.
  */
  Serial.println("Started firebase func");

  if (fb.setString("Example/myString", "Cool!") == 200){ 
    Serial.println("succes");
  }
  
  fb.setInt("Example/myInt", 123);
  fb.setFloat("Example/myFloat", 45.67);
  fb.setBool("Example/myBool", true);
  fb.pushString("Push", "Foo-Bar");
  fb.pushInt("Push", 890);
  fb.pushFloat("Push", 12.34);
  fb.pushBool("Push", false);

  /*
    Get String, Int, Float, or Bool from Firebase
    
    Parameters:
      - path: The path in Firebase from which the data will be retrieved.

    Returns:
      - The value retrieved from Firebase as a String, Int, Float, or Bool.
      - If the HTTP response code is not 200, returns NULL (for String) or 0 (for Int, Float, Bool).
  */
  String retrievedString = fb.getString("Example/myString");
  Serial.print("Retrieved String:\t");
  Serial.println(retrievedString);

  int retrievedInt = fb.getInt("Example/myInt");
  Serial.print("Retrieved Int:\t\t");
  Serial.println(retrievedInt);

  float retrievedFloat = fb.getFloat("Example/myFloat");
  Serial.print("Retrieved Float:\t");
  Serial.println(retrievedFloat);

  bool retrievedBool = fb.getBool("Example/myBool");
  Serial.print("Retrieved Bool:\t\t");
  Serial.println(retrievedBool);

  /*
    Remove Data from Firebase
    
    Parameters:
      - path: The path in Firebase from which the data will be removed.

    Returns:
      - HTTP response code as an integer.
        - 200 indicates success.
        - Other codes indicate failure.
  */
  fb.remove("Example");
  fb.remove("Push");
  }