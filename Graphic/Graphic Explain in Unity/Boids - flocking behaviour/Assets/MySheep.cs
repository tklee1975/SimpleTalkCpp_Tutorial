using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[DefaultExecutionOrder(100)]
public class MySheep : MonoBehaviour
{
    public float speed = 5;
    public float collisionRadius = 2;

    public Vector3 oldPosition;
    public Vector3 oldVelocity;

	public void Move(float deltaTime)
	{
        var pos = transform.position;
        var forward = transform.forward;

        var hit = new RaycastHit();
        if (Physics.Raycast(pos, forward, out hit, collisionRadius)) {
            forward -= hit.normal * Vector3.Dot(forward, hit.normal) * 1.02f;

            if (Vector3.SqrMagnitude(forward) > 0.01f) {
                transform.rotation = Quaternion.LookRotation(forward);
            }
            return;
        }

        pos += forward * (speed * Time.fixedDeltaTime);
        transform.position = pos;
	}
}
