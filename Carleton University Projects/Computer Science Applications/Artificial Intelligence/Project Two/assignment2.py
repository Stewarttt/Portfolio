# Connor Stewart (101041125)
# Gabrielle Latreille (101073284)
# Abdaljabbar Hersi (100919600)

# imported python modules
import math

# --------------------------- Naive Bayes Classifier -----------------------------------

# hard coded probability values and mu/sigma for each characteristic and each breed
beagle = {"probability": 0.3,
          "girth_mu": 41, "girth_sigma": 6,
          "height_mu": 37, "height_sigma": 4,
          "weight_mu": 10, "weight_sigma": 2}
corgi = {"probability": 0.21,
         "girth_mu": 53, "girth_sigma": 9,
         "height_mu": 27, "height_sigma": 3,
         "weight_mu": 12, "weight_sigma": 2}
huski = {"probability": 0.14,
         "girth_mu": 66, "girth_sigma": 10,
         "height_mu": 55, "height_sigma": 6,
         "weight_mu": 22, "weight_sigma": 6}
poodle = {"probability": 0.35,
          "girth_mu": 61, "girth_sigma": 9,
          "height_mu": 52, "height_sigma": 7,
          "weight_mu": 26, "weight_sigma": 8}


# probability density function as defined in the assignment
def probability_density_f(x, mu, sigma):
    return (1 / (math.sqrt(2 * math.pi * (sigma ** 2)))) * math.e ** (-(1 / 2) * (((x - mu) / sigma) ** 2))
# END probability_density_f


# calculate the probability of each characteristic for a specified breed
def get_probability(input, breed):
    # find the probability for these paramters
    girth_probability = probability_density_f(input[0], breed.get("girth_mu"), breed.get("girth_sigma"))
    height_probability = probability_density_f(input[1], breed.get("height_mu"), breed.get("height_sigma"))
    weight_probability = probability_density_f(input[2], breed.get("weight_mu"), breed.get("weight_sigma"))

    # return the combined probability (product of probability of eah characteristics * the probability of the breed)
    return girth_probability * height_probability * weight_probability * breed.get("probability")
# END get_probability


# calculate the most_likely class based on the class probabilities
def most_likely(class_probabilities):
    # if probability for all dogs is 0, return NA
    if all(dog == 0 for dog in class_probabilities):
        return "NA"
    # END IF

    # find the breed with the highest probability
    index = class_probabilities.index(max(class_probabilities))

    # test which category the classification fits into the best
    if index == 0:
        return "beagle"
    # END IF
    if index == 1:
        return "corgi"
    # END IF
    if index == 2:
        return "huski"
    # END IF
    if index == 3:
        return "poodle"
    # END IF
# END most_likely


def naive_bayes_classifier(input):
    # input is a three element list with [girth, height, weight]
    # calculate the probability of each breed
    beagle_probability = get_probability(input, beagle)
    corgi_probability = get_probability(input, corgi)
    huski_probability = get_probability(input, huski)
    poodle_probability = get_probability(input, poodle)

    # calculate the denominator
    denominator = beagle_probability + corgi_probability + huski_probability + poodle_probability

    # calculate the class probability for each breed / denominator calculated above
    class_probabilities = [beagle_probability / denominator,
                           corgi_probability / denominator,
                           huski_probability / denominator,
                           poodle_probability / denominator]

    # calculate the most likely class
    most_likely_class = most_likely(class_probabilities)

    # most_likely_class is a string indicating the most likely class, either "beagle", "corgi", "husky", or "poodle"
    # class_probabilities is a four element list indicating the probability of each class in the order [beagle probability, corgi probability, husky probability, poodle probability]
    return most_likely_class, class_probabilities
# END naive_bayes_classifier


# --------------------------- Fuzzy Classifier -----------------------------------

# hard code the values for a, b, c, d for each characteristic and fuzzy set
girth = {"small": [0, 0, 40, 50], "medium": [40, 50, 60, 70], "large": [60, 70, 100, 100]}
height = {"small": [0, 0, 25, 40], "medium": [25, 40, 50, 60], "large": [50, 60, 100, 100]}
weight = {"small": [0, 0, 5, 15], "medium": [5, 15, 20, 40], "large": [20, 40, 100, 100]}


# find the probability of x given a, b, c, d
def fuzzy_membership_f(x, a, b, c, d):
    if x <= a:
        return 0
    elif a < x < b:
        return (x - a) / (b - a)
    elif b <= x <= c:
        return 1
    elif c < x < d:
        return (d - x) / (d - c)
    elif d <= x:
        return 0
    # END IF
# END fuzzy_membership_f


# calculate the probability of the input belonging to the small, medium and large set given the input
def char_probability(input, char):
    # gets the fuzzy membership results as a function of the different dog sizes
    small = fuzzy_membership_f(input, *char["small"])
    medium = fuzzy_membership_f(input, *char["medium"])
    large = fuzzy_membership_f(input, *char["large"])

    # return the probabilities based off size
    return [small, medium, large]
# END char_probability


# finds the triangular norm for the data via the product t-norm
def t(x, y):
    return x * y
# END T


# finds the triangular conorm via the probabilistic sum
def s(x, y):
    return x + y - x * y
# END s


# calculate the probability of each breed given the fuzzy rules provided
# finds the fuzzy rules for the classification based off a product of the t-norm taken by
# the height, girth, or width along with the s-norm takes as a product of the other two parameters
def fuzzy_rules(g, h, w):
    # IF height is medium AND (girth is small OR weight is light) THEN beagle.
    # IF girth is medium AND height is short AND weight is medium THEN corgi.
    # IF girth is large AND height is tall AND weight is medium THEN husky.
    # IF (girth is medium OR height is medium) AND weight is heavy THEN poodle.
    rule_beagle = t(h[1], s(g[0], w[0]))
    rule_corgi = t(g[1], t(h[0], w[1]))
    rule_husky = t(g[2], t(h[2], w[1]))
    rule_poodle = t(s(g[1], h[1]), w[2])

    # return the fuzzy rules for the different dog breeds
    return [rule_beagle, rule_corgi, rule_husky, rule_poodle]
# END fuzzy_rules


def fuzzy_classifier(input):
    # input is a three element list with [girth, height, weight]
    g_p = char_probability(input[0], girth)
    h_p = char_probability(input[1], height)
    w_p = char_probability(input[2], weight)

    # determines the membership for the dog breeds in the given classes
    class_memberships = fuzzy_rules(g_p, h_p, w_p)

    # converts membership coefficient to dog breed name
    highest_membership_class = most_likely(class_memberships)

    # highest_membership_class is a string indicating the highest membership class, either "beagle", "corgi", "husky", or "poodle"
    # class_memberships is a four element list indicating the membership in each class in the order [beagle probability, corgi probability, husky probability, poodle probability]
    return highest_membership_class, class_memberships
# END fuzzy_classifier

