================================================================
VOLT SENTINEL — Smart Last-Mile Power Line Safety & Breakage Detection
Smart India Hackathon 2026
================================================================

Problem Statement ID : SIH25063
Theme                : Disaster Management
PS Category           : Hardware
Team Name             : GridNova

----------------------------------------------------------------
1. OVERVIEW
----------------------------------------------------------------
Volt Sentinel is a low-cost, distributed sensor network for
detecting breaks, leakage, and tree-contact faults on last-mile
electricity board (EB) power lines. Instead of relying on a single
sensor at the substation, it places a chain of sensor nodes along
the feeder and localizes faults by comparing readings between
neighbouring nodes.

----------------------------------------------------------------
2. PROBLEM
----------------------------------------------------------------
Last-mile power lines run for kilometres between substations and
end consumers. Faults such as snapped conductors, insulation
leakage, or tree contact are usually discovered only after a
complaint or a manual line-walk covering the entire stretch. This
causes:
  - Prolonged outages
  - Fire hazards from arcing or leakage
  - Safety risks to the public and maintenance crews
  - Long, unguided patrol distances to find the fault

----------------------------------------------------------------
3. PROPOSED SOLUTION
----------------------------------------------------------------
  - 20 sensor nodes deployed along a 10 km EB feeder (reference
    scenario).
  - Each node: current sensor + voltage sensor + ESP32.
  - Nodes send continuous telemetry to the EB office / server
    over Wi-Fi.
  - The server compares readings between adjacent nodes:
      * If later nodes go silent while earlier ones stay active,
        the break is localized to the span between the last
        active node and the first silent one
        (e.g. Nodes 1-15 active, 16-20 offline -> fault between
        Node 15 and Node 16).
      * If two adjacent, both-online nodes show a meaningful
        voltage/current difference, that span is flagged for
        leakage or tree contact.
  - Result: section-level fault detection instead of a full
    10 km manual search.

----------------------------------------------------------------
4. TECHNICAL APPROACH
----------------------------------------------------------------
Hardware:
  - Current sensor
  - Voltage sensor
  - ESP32 microcontroller
  - Power supply
  - Protected outdoor enclosure

Communication:
  - ESP32 Wi-Fi (WebSocket / HTTP REST telemetry)

Data flow:
  Sampling -> Transmission -> Node Comparison -> Fault Alert
  1. ESP32 measures voltage and current at each node.
  2. Readings are streamed to the central server.
  3. Server checks node status/continuity and runs differential
     analysis between adjacent nodes to identify the affected span.
  4. Dashboard raises an alert with location, fault type, and time.

----------------------------------------------------------------
5. FEASIBILITY AND RISK CONTROLS
----------------------------------------------------------------
  - Low-cost, readily available sensors and ESP32 hardware.
  - Modular design supports adding nodes for longer feeders.

  Risk               | Control
  -------------------|-----------------------------------------
  Weather exposure    | Sealed outdoor enclosure
  Lightning           | Surge protection and proper earthing
  Weak Wi-Fi signal   | Auto-reconnect routines
  Sensor error/drift  | Calibration and plausibility checks
  False alarms        | Persistence timer + adjacent-node check

  Pilot path: Bench test -> 3-node trial -> 20-node field
  deployment.

----------------------------------------------------------------
6. IMPACT AND BENEFITS
----------------------------------------------------------------
Users: EB operators, maintenance teams, industrial customers,
reliability planners.

  - Reduces fault search area from 10 km to a single span.
  - Shortens outage diagnosis and restoration time.
  - Identifies leakage and vegetation-contact points.
  - Reduces maintenance crew exposure/patrol distance.
  - Supports prevention of electrical fires and unsafe conditions.
  - Improves reliability reporting and customer updates.

Metrics tracked: node availability, alert time, false alarms,
patrol distance saved, restoration time.

----------------------------------------------------------------
7. FUTURE SCOPE
----------------------------------------------------------------
  - GPS/geo-tagging per node for precise alert coordinates.
  - Temperature/vibration sensing to distinguish thermal faults
    from mechanical strain or pure electrical leakage.
  - Historical trend views on the dashboard to flag gradual
    insulation drift before a hard fault occurs.
  - LoRa/GSM backhaul option for low Wi-Fi-coverage spans.
  - Multi-feeder support from a single EB office dashboard.

----------------------------------------------------------------
8. REFERENCES
----------------------------------------------------------------
  [1] CEA Safety and Electric Supply Regulations, 2023
  [2] ESP32 Wi-Fi & TCP/IP Networking Documentation
  [3] IEEE C37.114-2014 - AC Transmission/Distribution
      Fault-Location Guidance
  [4] IEEE 1366-2022 - Distribution Reliability Indices

----------------------------------------------------------------
9. TEAM
----------------------------------------------------------------
Team Name : GridNova
Event     : Smart India Hackathon 2026
================================================================
