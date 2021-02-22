using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityEngine;
using UnityEditor;

[CustomPropertyDrawer(typeof(Lesson013_TestClass), true)]
public class Lesson013_TestClass_PropDrawer : PropertyDrawer
{
	const float buttonHeight = 20;
	const float spacing = 4;

	public override void OnGUI(Rect position, SerializedProperty prop, GUIContent label)
	{
		var buttonRect = position;
		buttonRect.width  = 200;
		buttonRect.height = buttonHeight;

		if (GUI.Button(buttonRect, "Click Me")) {
			var msg = new StringBuilder();
			msg.Append("-- Click Me --\n");
			msg.Append($"path = [{prop.propertyPath}]\n");
			msg.Append($"type = [{prop.type}]\n");
			msg.Append($"hasMultipleDifferentValues = {prop.hasMultipleDifferentValues}]\n");

			var it = prop.Copy(); // don't change prop;
			var end = it.GetEndProperty();

			while (it.NextVisible(true)) {
				if (SerializedProperty.EqualContents(it, end)) break;

				msg.Append($"  - child [{it.propertyPath}]\n");
			}


			var c = prop.FindPropertyRelative("child1");
			if (c == null) {
				Debug.LogError("cannot find child prop");
			} else {
				if (c.type == "float") {
					c.floatValue = c.floatValue + 1;
				}
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