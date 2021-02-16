using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyPortal : MonoBehaviour
{
	public MyPortal target;

	private void OnDrawGizmos()
	{
		if (target) {
			Gizmos.color = Color.yellow;
			Gizmos.DrawLine(transform.position + new Vector3(0, 2, 0), target.transform.position);
		}
	}
}
