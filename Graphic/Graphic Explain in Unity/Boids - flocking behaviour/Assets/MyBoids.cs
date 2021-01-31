using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyBoids : MonoBehaviour
{
	List<MySheep> sheepList = new List<MySheep>();
	public int sheepCount = 10;
    public MySheep sheepPrefab;
	public float spawnRadius = 40;
	public Vector3 containerSize = new Vector3(110, 1, 110);

	public float randomSpeed = 1;
	public float maxSpeed = 5;

    public float randomSteering = 45;

	public float separationRadius = 3;
	public float separation = 1;

	public float aligmentRadius = 10;
	public float aligmentViewAngle = 120;
	public float alignment = 1;

	public float cohesionRadius = 20;
	public float cohesion = 1;

	public float changeRate = 0.1f;

	public int altalternativeUpdate = 4;
	int altalternativeUpdateIndex;

	float aligmentViewAngleCos;

	class Cell {
		public List<MySheep> sheepList;
		int _x, _y;

		public int x => _x;
		public int y => _y;

		public Cell(int x, int y) {
			sheepList = new List<MySheep>();
			_x = x;
			_y = y;
		}
	}

	Cell[] cellList;
	float cellSize;
	int cellCountX;
	int cellCountY;

	List<MySheep> tmpSheepList = new List<MySheep>();

	Cell cell(int x, int y) {
		if (x < 0 || x >= cellCountX) return null;
		if (y < 0 || y >= cellCountY) return null;
		return cellList[y * cellCountX + x];
	}

	Cell cellByPos(in Vector3 pos) {
		var offset = containerSize / 2;
		var p = (pos + offset) / cellSize;
		return cell(Mathf.FloorToInt(p.x), Mathf.FloorToInt(p.z));
	}

	void getSheepsNearby(ref List<MySheep> outList, Vector3 pos) {
		outList.Clear();

		var c = cellByPos(pos);
		if (c == null) return;
		
		{ var t = cell(c.x - 1, c.y - 1); if (t != null) outList.AddRange(t.sheepList); }
		{ var t = cell(c.x    , c.y - 1); if (t != null) outList.AddRange(t.sheepList); }
		{ var t = cell(c.x + 1, c.y - 1); if (t != null) outList.AddRange(t.sheepList); }

		{ var t = cell(c.x - 1, c.y    ); if (t != null) outList.AddRange(t.sheepList); }
		{ var t = cell(c.x    , c.y    ); if (t != null) outList.AddRange(t.sheepList); }
		{ var t = cell(c.x + 1, c.y    ); if (t != null) outList.AddRange(t.sheepList); }

		{ var t = cell(c.x - 1, c.y + 1); if (t != null) outList.AddRange(t.sheepList); }
		{ var t = cell(c.x    , c.y + 1); if (t != null) outList.AddRange(t.sheepList); }
		{ var t = cell(c.x + 1, c.y + 1); if (t != null) outList.AddRange(t.sheepList); }
	}

	void OnDrawGizmos()
	{
		if (cellList != null) {
			var offset = containerSize / 2;
			foreach (var c in cellList) {
				var size = Vector3.one * cellSize;
				var pos = new Vector3(c.x, 0, c.y) * cellSize - offset + size/2;
				Gizmos.DrawWireCube(pos, size);
				UnityEditor.Handles.Label(pos, $"{c.sheepList.Count}");
			}
		}
	}

	private void Start()
	{
		Application.targetFrameRate = 60;

		cellSize = Mathf.Max(aligmentRadius, cohesionRadius);
		{
			var div = containerSize / cellSize;
			cellCountX = Mathf.CeilToInt(div.x);
			cellCountY = Mathf.CeilToInt(div.z);
			cellList = new Cell[cellCountX * cellCountY];

			for (int y = 0; y < cellCountY; y++) {
				for (int x = 0; x < cellCountX; x++) {
					cellList[y * cellCountX + x] = new Cell(x, y);
				}
			}
		}

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
			sheepList.Add(s);
		}
	}

	private void Update()
	{
		foreach (var c in cellList) {
			c.sheepList.Clear();
		}

		foreach (var s in sheepList) {
			s.oldPosition = s.transform.localPosition;
			s.oldVelocity = s.transform.forward * s.speed;

			var c = cellByPos(s.oldPosition);
			if (c != null) {
				c.sheepList.Add(s);
			}
		}

		altalternativeUpdateIndex = (altalternativeUpdateIndex + 1) % altalternativeUpdate;

		for (int i = altalternativeUpdateIndex; i < sheepList.Count; i += altalternativeUpdate) {
			var s = sheepList[i];

			getSheepsNearby(ref tmpSheepList, s.oldPosition);
			SheepThink(s, Time.deltaTime, tmpSheepList);
		}
	}

	void SheepThink(MySheep sheep, float deltaTime, List<MySheep> nearbySheeps) {

		var separationPos = Vector3.zero;
		int separationCount = 0;

		var aligementVel = Vector3.zero;
		int aligementCount = 0;

		var cohesionPos = Vector3.zero;
		int cohesionCount = 0;

		foreach (var s in nearbySheeps) {
			var d = s.oldPosition - sheep.oldPosition;
			var dis = d.magnitude;

			if (s == sheep) continue;

			if (dis < separationRadius) {
				separationPos += s.oldPosition;
				separationCount++;
			}

			if (Vector3.Dot(d, sheep.transform.forward) > aligmentViewAngleCos) {
				if (dis < aligmentRadius) {
					aligementVel += s.oldVelocity;
					aligementCount++;
				}
			}

			if (dis < cohesionRadius) {
				cohesionPos += s.oldPosition;
				cohesionCount++;
			}
		}

		var vel = Vector3.zero;
	
		if (separationCount > 0) {
			separationPos /= separationCount;
			vel -= (separationPos - sheep.oldPosition).normalized * separation;
		}

		if (aligementCount > 0) {
			aligementVel /= aligementCount;
			vel += aligementVel * alignment;
		}

		if (cohesionCount > 0) {
			cohesionPos /= cohesionCount;
			vel += (cohesionPos - sheep.oldPosition).normalized * cohesion;
		}

		var rate = changeRate * deltaTime;
		vel = Vector3.Lerp(sheep.oldVelocity, vel, rate);

		vel = Quaternion.Euler(0, Random.Range(-randomSteering * rate, randomSteering * rate), 0) * vel;

		var speed = vel.magnitude;
		if (speed > 0.01f) {
			sheep.transform.localRotation = Quaternion.LookRotation(vel);
		}

		speed += Random.Range(-randomSpeed, randomSpeed);

		speed = Mathf.Lerp(sheep.speed, speed, rate);
		sheep.speed = Mathf.Clamp(speed, 0, maxSpeed);
	}
}
