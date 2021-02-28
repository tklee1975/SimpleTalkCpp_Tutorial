using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Lesson015_InputAction : MonoBehaviour
{
	UnityEngine.InputSystem.PlayerInput m_playerInput;
	private void Start()
	{
		m_playerInput = GetComponent<UnityEngine.InputSystem.PlayerInput>();
		m_playerInput.onActionTriggered += playerInput_onActionTriggered;
	}

	private void OnGUI()
	{
		var map = m_playerInput.currentActionMap;
		var moveDown = map.FindAction("Action_MoveDown");
		var attack = map.FindAction("Action_Attack");

		GUI.Box( new Rect(10, 440, 300, 200),
				"== Lesson015_InputAction ==\n"
				+ $"MoveDown = {moveDown.ReadValue<float>()}\n"
				+ $"Attack = {attack.ReadValue<float>()}\n");
	}

	//when PlayerInput.Behavior set to "Invoke C Sharp Events"
	private void playerInput_onActionTriggered(UnityEngine.InputSystem.InputAction.CallbackContext obj)
	{
		Debug.Log($"onActionTriggered {obj.action.name}");
	}

	void OnAction_MoveDown(UnityEngine.InputSystem.InputValue v) {
		Debug.Log("OnAction_MoveDown " + v.Get<float>());
	}

	void OnAction_Attack(UnityEngine.InputSystem.InputValue v) {
		Debug.Log("OnAction_Attack " + v.isPressed);
	}
}
