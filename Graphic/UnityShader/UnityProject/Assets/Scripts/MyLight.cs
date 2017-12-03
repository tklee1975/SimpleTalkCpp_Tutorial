using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class MyLight : MonoBehaviour {
	void Update () {

		var p = gameObject.GetComponent<Light>();
		if (p == null)
			return;
		Shader.SetGlobalVector("MyLightPos", p.transform.position);
	}
}
