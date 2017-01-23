#ifndef MAPKARMA_H_
#define MAPKARMA_H_

struct MapKarma
{
public:
    int Karma = 0;
    int MinVotes = 0;
    int PlusVotes = 0;
    double Percentage = 0.0;
    double MinPercentage = 0.0;

    void Calculate(std::map<std::string, int> votes)
    {
        MinVotes = 0;
        PlusVotes = 0;
        Percentage = 0.0;
        MinPercentage = 0.0;

        for(std::map<std::string, int>::iterator vote = votes.begin(); vote != votes.end(); ++vote)
        {
            if(vote->second == 1)
            {
                PlusVotes++;
            }
            else
            {
                MinVotes++;
            }
        }

        if((PlusVotes + MinVotes) > 0)
        {
            Percentage = std::round(((double)PlusVotes / (double)(PlusVotes + MinVotes)) * 1000) / 10;
            MinPercentage = std::round((100 - Percentage) * 10) / 10;
            Karma = (PlusVotes - MinVotes);
        }
    }
};

#endif // MAPKARMA_H_
