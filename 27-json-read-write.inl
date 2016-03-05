struct sMain : public ZL_Application
{
	ZL_Font fnt;

	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("JSON Reader and Writer", 854, 480);
		fnt = ZL_Font("Data/fntMain.png");
	}

	void AfterFrame()
	{
		ZL_Display::ClearFill();

		//Initialize a JSON object from a string with 3 keys and values
		ZL_Json Test("{ \"string\" : \"text\", \"number\" : 10, \"boolean\" : true }");

		//Add a fourth key "array" with 3 elements, change the third element to another number and then remove the second element
		Test["array"] = ZL_Json("[1, 2, 3]");
		Test["array"](2) = 333;
		Test["array"].EraseAt(1);

		//Load a file (not a file from disk, just 10 bytes from memory) and add it as another key
		ZL_File MemoryJSONFile("\"jsontest\"", 10);
		Test["from_other_file"] = ZL_Json(MemoryJSONFile);

		//This adds key "a" with the JSON object { "b" : { "c" : 123 } } and then changes "b" to "bbb"
		Test["a"]["b"]["c"] = 123;
		Test["a"]["b"].SetKey("bbb");

		//Erase an object key-value
		Test.Erase("number");

		//Loop through all keys now present in Test and put them into a separate JSON array which afterwards is added as "my_keys"
		ZL_Json JSONKeysArray;
		for (ZL_Json& it : Test) JSONKeysArray.Add().SetString(it.GetKey());
		Test["my_keys"] = JSONKeysArray;

		//Final JSON:
		/* {
			"string" : "text",
			"boolean" : true,
			"array" : [ 1, 333 ],
			"from_other_file" : "jsontest",
			"a" : { "bbb" : { "c" : 123 } },
			"my_keys" : [ "string", "boolean", "array", "from_other_file", "a" ]
		} */

		//Show JSON as string on screen
		fnt.Draw(ZLCENTER, Test.ToString(), ZL_Origin::Center);
	}
} Main;
