using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[SelectionBase]
public class MySheep : MonoBehaviour
{
    public float speed = 5;
    public float collisionRadius = 2;

    public Vector3 oldPosition;
    public Vector3 oldVelocity;

	public void Update()
	{
        var pos = transform.localPosition;
        var forward = transform.forward;

        var hit = new RaycastHit();
        if (Physics.Raycast(pos, forward, out hit, collisionRadius)) {
            forward -= hit.normal * Vector3.Dot(forward, hit.normal);

            if (Vector3.SqrMagnitude(forward) < 0.01f) {
                forward = Time.frameCount % 2 == 0 ? transform.right : -transform.right;
            }

            transform.localRotation = Quaternion.LookRotation(forward);
            return;
        }

        pos += forward * (speed * Time.deltaTime);
        transform.localPosition = pos;
	}
}
