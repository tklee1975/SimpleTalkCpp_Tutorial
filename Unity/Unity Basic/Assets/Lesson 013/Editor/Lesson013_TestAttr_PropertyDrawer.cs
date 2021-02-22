using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityEngine;
using UnityEditor;

[CustomPropertyDrawer(typeof(Lesson013_TestAttribute), true)]
public class Lesson013_TestAttr_PropDrawer : PropertyDrawer
{
	const int buttonHeight = 20;
	const int spacing = 4;

	public override void OnGUI(Rect position, SerializedProperty prop, GUIContent label)
	{
		var buttonRect = position;
		buttonRect.width  = 140;
		buttonRect.height = buttonHeight;

		if (GUI.Button(buttonRect, "Click Me")) {
			var msg = new StringBuilder();
			msg.Append("-- Click Me --\n");
			msg.Append($"path = [{prop.propertyPath}]\n");
			msg.Append($"type = [{prop.type}]\n");
			msg.Append($"hasMultipleDifferentValues = {prop.hasMultipleDifferentValues}]\n");

			msg.Append($"NicifyVariableName = [{ObjectNames.NicifyVariableName("m_testABC")}");

			var attr = this.attribute as Lesson013_TestAttribute;
			if (attr != null) {
				msg.Append($"Attribute message = [{attr.message}]\n");
			}


			if (prop.type == "int") {
				prop.intValue = prop.intValue + 1;
			}

			Debug.Log(msg.ToString());
		}

		position.y = buttonRect.yMax + spacing;
		position.height = EditorGUI.GetPropertyHeight(prop, true);

		EditorGUI.PropertyField(position, prop, true);
	}

	public override float GetPropertyHeight(SerializedProperty prop, GUIContent label)
	{
		return EditorGUI.GetPropertyHeight(prop, true) + spacing + buttonHeight;
	}

}