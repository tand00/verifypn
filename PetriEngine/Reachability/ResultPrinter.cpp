
#include "ReachabilityResult.h"
#include "../PetriNetBuilder.h"
#include "../options.h"

namespace PetriEngine {
    namespace Reachability {
        ResultPrinter::Result ResultPrinter::printResult(
                size_t index,
                PQL::Condition* query, 
                ResultPrinter::Result result,
                size_t expandedStates,
                size_t exploredStates,
                size_t discoveredStates,
                const std::vector<size_t> enabledTransitionsCount,
                int maxTokens,
                const std::vector<uint32_t> maxPlaceBound )
        {
            if(result == Unknown) return Unknown;
            Result retval = result;
            std::cout << std::endl;            
            if(!options->statespaceexploration && retval != Unknown)
            {
                std::cout << "FORMULA " << querynames[index] << " ";
            }
            else {
                retval = Satisfied;
                uint32_t placeBound = 0;
                for (size_t p = 0; p < maxPlaceBound.size(); p++) {
                    placeBound = std::max<uint32_t>(placeBound, maxPlaceBound[p]);
                }
                // fprintf(stdout,"STATE_SPACE %lli -1 %d %d TECHNIQUES EXPLICIT\n", result.exploredStates(), result.maxTokens(), placeBound);
                std::cout   << "STATE_SPACE STATES "<< exploredStates <<" TECHNIQUES EXPLICIT\n" 
//                            << "STATE_SPACE TRANSITIONS "<< discoveredStates <<" TECHNIQUES EXPLICIT\n" 
                            << "STATE_SPACE TRANSITIONS "<< -1 <<" TECHNIQUES EXPLICIT\n" 
                            << "STATE_SPACE MAX_TOKEN_PER_MARKING "<< maxTokens << " TECHNIQUES EXPLICIT\n" 
                            << "STATE_SPACE MAX_TOKEN_IN_PLACE "<< placeBound <<" TECHNIQUES EXPLICIT\n"
                            << std::endl;
                return retval;
            }
            
            if (result == Satisfied)
                retval = query->isInvariant() ? NotSatisfied : Satisfied;
            else if (result == NotSatisfied)
                retval = query->isInvariant() ? Satisfied : NotSatisfied;

            //Print result
            if (retval == Unknown)
            {
                std::cout << "\nUnable to decide if " << querynames[index] << " is satisfied.";
            }
            else if (retval == Satisfied) {
                if(!options->statespaceexploration)
                {
                    std::cout << "TRUE TECHNIQUES EXPLICIT ";
                    if(options->enablereduction > 0)
                    {
                        std::cout << "STRUCTURAL_REDUCTION";
                    }
                    std::cout << std::endl;
                }
            } else if (retval == NotSatisfied) {
                if (!query->placeNameForBound().empty()) {
                    // find index of the place for reporting place bound
                    size_t bound = 0;
                    for(auto& p : query->placeNameForBound())
                    {
                        bound += builder->getPlaceNames().at(p);
                    }
                    std::cout << bound <<  " TECHNIQUES EXPLICIT ";
                    
                    if(options->enablereduction > 0)
                    {
                        std::cout << "STRUCTURAL_REDUCTION";
                    }
                } else {
                    if(!options->statespaceexploration)
                    {
                        std::cout << "FALSE TECHNIQUES EXPLICIT ";
                        if(options->enablereduction > 0)
                        {
                            std::cout << "STRUCTURAL_REDUCTION";
                        }
                    }
                }
            }
            std::cout << std::endl;
            return retval;
        }            

    }
}