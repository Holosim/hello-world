#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct ChoiceSet {
    std::string label;
    std::vector<std::string> options;
};

struct Build {
    int greeting = 0;
    int target = 0;
    int tone = 0;
};

struct Level {
    std::string title;
    std::string species;
    std::string briefing;
    std::vector<std::string> rules;
    std::vector<std::string> hints;
    std::vector<ChoiceSet> parts;
    Build solution;
    std::vector<std::string> failureNotes;
    std::string successText;
};

int askInt(int minValue, int maxValue) {
    while (true) {
        std::cout << "> ";
        int value = 0;
        if (std::cin >> value && value >= minValue && value <= maxValue) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a number from " << minValue << " to " << maxValue << ".\n";
    }
}

std::string trim(std::string text) {
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) {
        text.erase(text.begin());
    }
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) {
        text.pop_back();
    }
    return text;
}

std::string buildTransmission(const Level& level, const Build& build) {
    std::string line =
        level.parts[0].options[build.greeting] + " " +
        level.parts[1].options[build.target] + " " +
        level.parts[2].options[build.tone];

    return trim(line);
}

void printDivider() {
    std::cout << "\n------------------------------------------------------------\n";
}

void showRules(const Level& level) {
    printDivider();
    std::cout << "Protocol notes for " << level.species << ":\n";
    for (const auto& rule : level.rules) {
        std::cout << " - " << rule << '\n';
    }
}

void showOptions(const Level& level) {
    printDivider();
    for (const auto& part : level.parts) {
        std::cout << part.label << ":\n";
        for (std::size_t i = 0; i < part.options.size(); ++i) {
            std::cout << " " << (i + 1) << ". " << part.options[i] << '\n';
        }
        std::cout << '\n';
    }
}

Build chooseTransmission(const Level& level) {
    Build build;
    printDivider();
    std::cout << "Construct your transmission.\n";
    for (std::size_t i = 0; i < level.parts.size(); ++i) {
        const auto& part = level.parts[i];
        std::cout << '\n' << part.label << ":\n";
        for (std::size_t optionIndex = 0; optionIndex < part.options.size(); ++optionIndex) {
            std::cout << " " << (optionIndex + 1) << ". " << part.options[optionIndex] << '\n';
        }

        int choice = askInt(1, static_cast<int>(part.options.size())) - 1;
        if (i == 0) {
            build.greeting = choice;
        } else if (i == 1) {
            build.target = choice;
        } else {
            build.tone = choice;
        }
    }
    return build;
}

int mismatches(const Level& level, const Build& build) {
    int misses = 0;
    misses += (build.greeting != level.solution.greeting);
    misses += (build.target != level.solution.target);
    misses += (build.tone != level.solution.tone);
    return misses;
}

void showHint(const Level& level, const Build& build) {
    printDivider();
    std::cout << "Analysis:\n";

    if (build.greeting != level.solution.greeting) {
        std::cout << " - Greeting form mismatch: " << level.failureNotes[0] << '\n';
    }
    if (build.target != level.solution.target) {
        std::cout << " - Target noun mismatch: " << level.failureNotes[1] << '\n';
    }
    if (build.tone != level.solution.tone) {
        std::cout << " - Tone or protocol mismatch: " << level.failureNotes[2] << '\n';
    }

    const int hintIndex = std::min(mismatches(level, build) - 1, static_cast<int>(level.hints.size()) - 1);
    if (hintIndex >= 0) {
        std::cout << "Hint: " << level.hints[hintIndex] << '\n';
    }
}

bool playLevel(const Level& level, int& score) {
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts) {
        printDivider();
        std::cout << level.title << "\nSpecies: " << level.species << "\n\n" << level.briefing << "\n\n";
        std::cout << "1. Inspect protocol notes\n";
        std::cout << "2. Review phrase components\n";
        std::cout << "3. Send a transmission\n";
        std::cout << "4. Abort mission\n";

        const int action = askInt(1, 4);
        if (action == 1) {
            showRules(level);
            continue;
        }
        if (action == 2) {
            showOptions(level);
            continue;
        }
        if (action == 4) {
            return false;
        }

        Build build = chooseTransmission(level);
        ++attempts;

        printDivider();
        std::cout << "Transmission sent: \"" << buildTransmission(level, build) << "\"\n";

        if (mismatches(level, build) == 0) {
            const int bonus = maxAttempts - attempts + 2;
            score += bonus;
            std::cout << "\n" << level.successText << "\n";
            std::cout << "Diplomatic rating: +" << bonus << " coherence\n";
            return true;
        }

        std::cout << "\nThe translation probe emits a deeply worrying alarm.\n";
        showHint(level, build);
        std::cout << "Attempts remaining: " << (maxAttempts - attempts) << "\n";
    }

    printDivider();
    std::cout << "Mission failure. The probe has been reclassified as a cultural weapon.\n";
    return false;
}

std::vector<Level> makeLevels() {
    return {
        {
            "Level 1: The Syntax Union",
            "Auditors of the Runtime Gate",
            "The Union accepts greetings only in valid protocol form. They consider small talk a parsing error.",
            {
                "Start with a formal handshake token.",
                "Address the runtime, not a literal planet.",
                "Close with a clean validation marker."
            },
            {
                "They care about structure more than warmth.",
                "In this culture, 'world' usually means the execution environment.",
                "A missing validation marker reads as social corruption."
            },
            {
                { "Greeting fragment", { "HELLO::", "hey", "greetings??" } },
                { "Target fragment", { "runtime", "planet", "everyone" } },
                { "Protocol suffix", { "[ok]", "!!!", "(maybe)" } }
            },
            { 0, 0, 0 },
            {
                "The auditors reject sloppy or casual opening syntax.",
                "You addressed the wrong kind of 'world.'",
                "Their etiquette requires an explicit success marker."
            },
            "The gate unfurls. A thousand robotic clerks stamp your greeting as syntactically delightful."
        },
        {
            "Level 2: Chorus Without End",
            "The Polyphonic Swarm",
            "The swarm shares one mind across billions of bodies. Singular phrasing is considered tragically lonely.",
            {
                "Greet collectively.",
                "Address the cluster, not an individual world.",
                "Close in a way that implies harmony."
            },
            {
                "Any singular perspective sounds like a threat.",
                "The swarm values togetherness more than geography.",
                "Harmony is not optional. It is grammar."
            },
            {
                { "Greeting fragment", { "we greet", "I greet", "hello there" } },
                { "Target fragment", { "cluster", "world", "you" } },
                { "Protocol suffix", { "in chorus", "alone", "at random" } }
            },
            { 0, 0, 0 },
            {
                "They need a plural, collective opening.",
                "The correct audience is the whole chorus.",
                "Your closing did not sound unified enough."
            },
            "The swarm hums back in twelve thousand-part harmony. Somehow, it feels like applause."
        },
        {
            "Level 3: The Bound Sphere",
            "Court of the Cartographer-King",
            "In this kingdom, the word 'world' is the sacred name of a sleeping god. Use it casually and you are legally engaged to the apocalypse.",
            {
                "Use a respectful greeting.",
                "Address the realm, never the forbidden word.",
                "Accompany the phrase with a ritual bow marker."
            },
            {
                "This level is mostly about not waking a deity.",
                "Their safe synonym is territorial, not cosmic.",
                "The court trusts posture as much as language."
            },
            {
                { "Greeting fragment", { "well met", "yo", "HELLO::" } },
                { "Target fragment", { "realm", "world", "map" } },
                { "Protocol suffix", { "<bow>", "<wink>", "<sneeze>" } }
            },
            { 0, 0, 0 },
            {
                "The court expects something ceremonious.",
                "You may have invoked the forbidden noun.",
                "The body language packet is wrong."
            },
            "The king nods once. Priests quietly cancel the emergency volcano preparations."
        },
        {
            "Level 4: The Optimizing Abbey",
            "Monks of the Sacred Compiler",
            "These monks believe wasteful language traps souls in unnecessary loops. They admire brevity, precision, and constant-time peace.",
            {
                "Choose the smallest acceptable greeting.",
                "Address the execution environment.",
                "Close with a statement of efficient intent."
            },
            {
                "Verbose greetings are a moral failure here.",
                "Their 'world' is definitely not geological.",
                "They want elegance, not drama."
            },
            {
                { "Greeting fragment", { "hi", "salutations honorable entities", "well met" } },
                { "Target fragment", { "environment", "galaxy", "world" } },
                { "Protocol suffix", { "O(1) peace", "with excessive enthusiasm", "eventually" } }
            },
            { 0, 0, 0 },
            {
                "Try the leanest valid opening.",
                "A compiler monastery prefers technical context.",
                "Efficiency is part of the blessing."
            },
            "The monks ring a tiny bell exactly once. This is, apparently, rapturous approval."
        }
    };
}

int main() {
    std::cout << "HELLO, WORLD?\n";
    std::cout << "A first-contact bureaucracy game\n";
    std::cout << "\nYou are the linguistic interpreter aboard humanity's least reliable diplomacy probe.\n";
    std::cout << "Your mission: make four civilizations accept a version of \"hello world\"\n";
    std::cout << "without starting a war, a religion, or a compiler warning.\n";

    std::vector<Level> levels = makeLevels();
    int score = 0;

    for (const auto& level : levels) {
        if (!playLevel(level, score)) {
            std::cout << "\nFinal coherence score: " << score << "\n";
            std::cout << "Status: humanity will be remembered, but not fondly.\n";
            return 0;
        }
    }

    printDivider();
    std::cout << "Campaign complete.\n";
    std::cout << "Final coherence score: " << score << "\n";

    if (score >= 11) {
        std::cout << "Outcome: You are now Ambassador of Impossible Greetings.\n";
    } else if (score >= 8) {
        std::cout << "Outcome: Peace achieved, though several cultures remain confused.\n";
    } else {
        std::cout << "Outcome: The galaxy accepts your greeting with mild legal reservations.\n";
    }

    std::cout << "\nThanks for playing.\n";
    return 0;
}
