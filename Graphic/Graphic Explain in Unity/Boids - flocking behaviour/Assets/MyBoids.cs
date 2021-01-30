using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyBoids : MonoBehaviour
{
	List<MySheep> sheeps = new List<MySheep>();
	public int sheepCount = 10;
	public float spawnRadius = 20;
    public MySheep sheepPrefab;

	public float randomSpeed = 1;
	public float maxSpeed = 5;

    public float randomSteering = 1;

	public float separationRadius = 3;
	public float separation = 1;

	public float aligmentRadius = 10;
	public float aligmentViewAngle = 120;
	public float alignment = 1;

	public float cohesionRadius = 20;
	public float cohesion = 1;

	public float changeRate = 0.1f;

	float aligmentViewAngleCos;

	private void Start()
	{
		Application.targetFrameRate = 60;

		aligmentViewAngleCos = Mathf.Cos(Mathf.Deg2Rad * aligmentViewAngle * 0.5f);

		if (!sheepPrefab) {
			Debug.LogError("Missing SheepPrefab");
			return;
		}

		for (int i = 0; i < sheepCount; i++) {
			var r = Random.insideUnitCircle * spawnRadius;
			var pos = new Vector3(r.x, 0, r.y);
			var rot = Quaternion.Euler(0, Random.Range(0, 360), 0);

			var o = Instantiate(sheepPrefab.gameObject, pos, rot, transform);
			o.name = "Sheep" + i;
			var s = o.GetComponent<MySheep>();
			sheeps.Add(s);
		}
	}

	private void Update()
	{
		var dt = Time.deltaTime;

		foreach (var s in sheeps) {
			s.oldPosition = s.transform.position;
			s.oldVelocity = s.transform.forward * s.speed;
		}

		foreach (var s in sheeps) {
			SheepThink(s, dt);
			s.Move(dt);
		}
	}

	void SheepThink(MySheep sheep, float deltaTime) {

		var cohesionPos = Vector3.zero;
		int cohesionCount = 0;

		var separationPos = Vector3.zero;
		int separationCount = 0;

		var aligementVel = Vector3.zero;
		int aligementCount = 0;

		foreach (var s in sheeps) {
			var d = s.oldPosition - sheep.oldPosition;
			var dis = d.magnitude;

			if (s == sheep) continue;

			if (dis < cohesionRadius) {
				cohesionPos += s.oldPosition;
				cohesionCount++;
			}


			if (dis < separationRadius) {
				separationPos += s.oldPosition;
				separationCount++;
			}

			if (dis < aligmentRadius) {
				if (Vector3.Dot(d, sheep.transform.forward) > aligmentViewAngleCos) {
					aligementVel += s.oldVelocity;
					aligementCount++;
				}
			}
		}

		var vel = Vector3.zero;

		if (cohesionCount > 0) {
			cohesionPos /= cohesionCount;
			vel += (cohesionPos - sheep.oldPosition).normalized * cohesion;
		}
		
		if (separationCount > 0) {
			separationPos /= separationCount;
			vel -= (separationPos - sheep.oldPosition).normalized * separation;
		}

		if (aligementCount > 0) {
			aligementVel /= aligementCount;
			vel += aligementVel * alignment;
		}

		vel = Quaternion.Euler(0, Random.Range(-randomSteering, randomSteering), 0) * vel;

		vel = Vector3.Lerp(sheep.oldVelocity, vel, changeRate * deltaTime);

		var m = vel.magnitude;
		if (m > float.Epsilon) {
			sheep.transform.rotation = Quaternion.LookRotation(vel);
		}

		var speed = Mathf.Clamp(m + Random.Range(-randomSpeed, randomSpeed), 0, maxSpeed);
		sheep.speed = Mathf.Lerp(sheep.speed, speed, changeRate * deltaTime);
	}
}
