using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ex007_RenderToTexture : MonoBehaviour {

	public Material mat;
	public Camera renderTexCamera;

	public RenderTexture renderTex;

	private void Update() {
		if (renderTex == null || renderTex.width != Screen.width || renderTex.height != Screen.height) {
			if (renderTex)
				renderTex.Release();
			renderTex = new RenderTexture(Screen.width, Screen.height, 24, RenderTextureFormat.ARGB32);
		}

		if (renderTexCamera)
			renderTexCamera.targetTexture = renderTex;

		if (mat)
			mat.SetTexture("_MainTex", renderTex);
	}

	private void OnRenderImage(RenderTexture src, RenderTexture dst) {
		if (!mat || !renderTex)
			return;

		mat.SetTexture("inputTex", renderTex);
		Graphics.Blit(src, dst, mat);
	}
}
