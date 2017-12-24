using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyUtil {

	static public void Destroy(ref Material v) {
		if (!v)
			return;
		Object.Destroy(v);
		v = null;
	}

	static public void Destroy(ref RenderTexture v) {
		if (!v)
			return;
		Object.Destroy(v);
		v = null;
	}

	static public void Destroy(ref Mesh v) {
		if (!v)
			return;
		Object.Destroy(v);
		v = null;
	}
}
