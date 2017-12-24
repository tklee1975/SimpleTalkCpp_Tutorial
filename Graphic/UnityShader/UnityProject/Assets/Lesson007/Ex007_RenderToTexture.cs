using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ex007_RenderToTexture : MonoBehaviour {

	public Material mat;
	public RenderTexture renderTex;

	private void Update() {
		if (renderTex == null || renderTex.width != Screen.width || renderTex.height != Screen.height) {
			renderTex = new RenderTexture(Screen.width, Screen.height, 24, RenderTextureFormat.ARGB32);
		}

		if (!transform.parent)
			return;

		var p = transform.parent.GetComponent<Camera>();	
		if (!p)
			return;

		p.targetTexture = renderTex;

		if (mat)
			mat.SetTexture("_MainTex", renderTex);
	}

}
