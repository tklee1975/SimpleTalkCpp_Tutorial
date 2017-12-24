using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ex007_Blur_Camera : MonoBehaviour {

	public enum Example {
		Blur3x3,
		Blur5x5,
		Blur7x7,

		Blur3x3_TwoPass,
		Blur5x5_TwoPass,
		Blur7x7_TwoPass,
	};

	public Example example = Example.Blur7x7;

	public Shader   blur3x3Shader;
	public Shader   blur5x5Shader;
	public Shader   blur7x7Shader;

	[Range(0,1)]
	public float intensity = 0;

	[Range(0,16)]
	public float blur1 = 1;

	[Range(0,2)]
	public float blur2 = 1.6f;

	private Material blur3x3Mat1;
	private Material blur3x3Mat2;

	private Material blur5x5Mat1;
	private Material blur5x5Mat2;

	private Material blur7x7Mat1H;
	private Material blur7x7Mat1V;

	private Material blur7x7Mat2H;
	private Material blur7x7Mat2V;

	public RenderTexture renderTex;
	public RenderTexture renderTex2;

	private void Start() {
		blur3x3Mat1 = new Material(blur3x3Shader);
		blur3x3Mat2 = new Material(blur3x3Shader);

		blur5x5Mat1 = new Material(blur5x5Shader);
		blur5x5Mat2 = new Material(blur5x5Shader);

		blur7x7Mat1H = new Material(blur7x7Shader);
		blur7x7Mat1V = new Material(blur7x7Shader);
		blur7x7Mat2H = new Material(blur7x7Shader);
		blur7x7Mat2V = new Material(blur7x7Shader);
	}

	void OnRenderImage(RenderTexture src, RenderTexture dst) {

		if (renderTex == null || renderTex.width != src.width || renderTex.height != src.height) {
			MyDestroyObject(ref renderTex);
			Debug.Log("create renderTex");
			renderTex = new RenderTexture(src.width, src.height, 0);
		}

		if (renderTex2 == null || renderTex2.width != src.width || renderTex2.height != src.height) {
			MyDestroyObject(ref renderTex2);
			renderTex2 = new RenderTexture(src.width, src.height, 0);
		}

		switch (example) {
			case Example.Blur3x3: {
				blur3x3Mat1.SetFloat("intensity", intensity * blur1);
				Graphics.Blit(src, dst, blur3x3Mat1);
			} break;

			case Example.Blur5x5: {
				blur5x5Mat1.SetFloat("intensity", intensity * blur1);
				Graphics.Blit(src, dst, blur5x5Mat1);
			} break;

			case Example.Blur3x3_TwoPass: {
				blur3x3Mat1.SetFloat("intensity", intensity * blur1);
				blur3x3Mat2.SetFloat("intensity", intensity * blur1 * blur2);

				Graphics.Blit(src, renderTex, blur3x3Mat1);
				Graphics.Blit(renderTex, dst, blur3x3Mat2);
			} break;

			case Example.Blur5x5_TwoPass: {
				blur5x5Mat1.SetFloat("intensity", intensity * blur1);
				blur5x5Mat2.SetFloat("intensity", intensity * blur1 * blur2);

				Graphics.Blit(src, renderTex, blur5x5Mat1);
				Graphics.Blit(renderTex, dst, blur5x5Mat2);
			} break;

			case Example.Blur7x7: {
				blur7x7Mat1H.SetVector("intensity", new Vector4(1,0,0,0) * intensity * blur1);
				blur7x7Mat1V.SetVector("intensity", new Vector4(0,1,0,0) * intensity * blur1);

				Graphics.Blit(src, renderTex, blur7x7Mat1H);
				Graphics.Blit(renderTex, dst, blur7x7Mat1V);
			} break;

			case Example.Blur7x7_TwoPass: {
				blur7x7Mat1H.SetVector("intensity", new Vector4(1,0,0,0) * intensity * blur1);
				blur7x7Mat1V.SetVector("intensity", new Vector4(0,1,0,0) * intensity * blur1);

				blur7x7Mat2H.SetVector("intensity", new Vector4(1,0,0,0) * intensity * blur1 * blur2);
				blur7x7Mat2V.SetVector("intensity", new Vector4(0,1,0,0) * intensity * blur1 * blur2);

				Graphics.Blit(src, renderTex, blur7x7Mat1H);
				Graphics.Blit(renderTex, renderTex2, blur7x7Mat1V);

				Graphics.Blit(renderTex2, renderTex, blur7x7Mat2H);
				Graphics.Blit(renderTex, dst, blur7x7Mat2V);
			} break;
		}
	}

	void MyDestroyObject(ref Material m) {
		if (m) {
			Object.Destroy(m);
			m = null;
		}
	}

	void MyDestroyObject(ref RenderTexture m) {
		if (m) {
			Object.Destroy(m);
			m = null;
		}
	}

	private void OnDestroy() {
		MyDestroyObject(ref blur3x3Mat1);
		MyDestroyObject(ref blur3x3Mat2);

		MyDestroyObject(ref blur5x5Mat1);
		MyDestroyObject(ref blur5x5Mat2);

		MyDestroyObject(ref blur7x7Mat1H);
		MyDestroyObject(ref blur7x7Mat1V);
		MyDestroyObject(ref blur7x7Mat2H);
		MyDestroyObject(ref blur7x7Mat2V);

		MyDestroyObject(ref renderTex);
		MyDestroyObject(ref renderTex2);
	}
}
