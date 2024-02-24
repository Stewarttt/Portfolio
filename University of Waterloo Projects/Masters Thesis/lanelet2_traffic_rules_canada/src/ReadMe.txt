Introduction:
    The following C++ files CanadianTrafficRules.cpp and ExtendedTrafficRules.cpp are used for building custom rulesets to evaluate
    traffic rules. Rules are built from the Lanelet2 library and the project currently supports:
        -> Runtime analysis of rule evaluation using static test cases.
        -> Recording analysis of rule evaluation using dynamic test cases consisting of interaction datasets.
Running the program (ExtendedTrafficRules.cpp):
    To run the following package, read the README.md file in the parent directory of this files folder.
        To run test cases, change the directory to the /Lanelet2-ws folder and run:
            >> catkin build
            >> catkin run_tests
        A set of testing scripts will be evaluated:
            -> Location: ./Lanelet2-ws/src/lanelet2_traffic_rules_canada/lanelet2_traffic_rules_canada/test/lanelet2_traffic_rules.cpp
        Failed test cases indicate that the vehicle with the specified ID from the given interaction dataset has violated the
        specified rule. The specific interaction dataset evaluated can be determined from an error message above the errors 
        for the vehicles in the dataset.
References (ExtendedTrafficRules.cpp):
    Various methods are used throughout the program, with links to each located both in this ReadMe file and in the codes comments.
    Minimum safe driving distance (d_min) is calcualted using Intels Safe Longitudinal Distance - Same Direction formula:
        https://arxiv.org/pdf/1708.06374.pdf
    Interaction dataset formatting guidelines can be found here:
        https://interaction-dataset.com/details-and-format
    Traffic rules are formaulated in Formalization of Intersection Traffic Rules in Temporal Logic, which can be found at:
        https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
    Information on specific useage of the above references is located in the code comments.
Notes:
    The code relys on the use of OSM files that are properly formatted to determine the identity of street signs and lane rules.
    The code relys on properly formatted datasets for reading interaction dataset information as well.
    Visualization of results to check rule the flagged rule violations real-time at the intersection can be achived using:
        https://github.com/interaction-dataset/interaction-dataset
CanadianTrafficRules:
    Canadian traffic rules are formalized using a canadian traffic rule C++ file for initializing speed limits and traffic signs for
    Canadian vehicles.
    Canadian traffic signs and standard are formatted according to information found at the following:
        https://en.wikipedia.org/wiki/Road_signs_in_Canada
        https://en.wikipedia.org/wiki/Speed_limits_in_Canada