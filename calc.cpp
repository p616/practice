#include <iostream>
#include <string>
#include <cstring>
#include <array>
#include <queue>

//1/0=
//1+2*3*4-1=
//8.7+-2.2*3=
//2*3+5*6/2-1=
//Q

using namespace std;

bool DoTheMath(char op, double x, double y, double &result)
{
    bool mathError = false;
    
    result = 0.0;
    
    // TODO: integer overflow/underflow
    
    switch (op)
    {
        case '*' : result = x*y; break;
        case '/' : 
            if (y==0)
            {
                mathError = true;
            }
            else
            {
                result = x/y; 
            }
            break;
        case '+' : result = x+y; break;
        case '-' : result = x-y; break;
        default:
            break;
    }
    
    return !mathError;
}

bool ProcessOp(char op, queue<char> &opQ, queue<double> &numQ)
{
    bool success = true;
    
    cout << "\n\nProcessOp(" << op << "):";
    
    int numOps = opQ.size();
    
    for (int i=0; i<numOps; i++) 
    {
        char currentOp = opQ.front();
        opQ.pop();
        
        cout << "\ncurrentOp=" << currentOp;
        
        if (currentOp == op)
        {
            // Calculate interim result and requeue
            double result = 0.0;
            
            double x = numQ.front();
            numQ.pop();
            double y = numQ.front();
            numQ.pop();
            
            cout << " x=" << x << " y=" << y;
            
            if (!DoTheMath(currentOp, x, y, result))
            {
                success = false;
                break;
            }
            else
            {
                cout << " result=" << result;
                numQ.push(result);
            }

        }
        else
        {
            // Skip and requeue this op + corresponding num
            opQ.push(currentOp);
            numQ.push(numQ.front());
            numQ.pop();
        }
    }
    
    numQ.push(numQ.front());
    numQ.pop();

    return success;
}

bool Evaluate(queue<char> &opQ, queue<double> &numQ)
{
    bool success = true;
    
    // Process unaryOps_back
    array<const char, 1> unaryOps_back { '!' };
    
    // Process unaryOps_front
    array<string, 4> unaryOps_front { "log", "cos", "sin", "tan" };

    // Process binaryOps (in order of precedence : PEDMAS)  
    array<const char, 4> binaryOps { /*'^'*/ '*', '/', '+', '-' };  
    
    for (auto op : binaryOps)
    {
        if (!ProcessOp(op, opQ, numQ))
        {
            success = false;
            break;
        }
    }
    
    success = (success && opQ.empty() && numQ.size() == 1);
    
    return success;
}
    

bool ParseExpression(string &expr, queue<char> &opQ, queue<double> &numQ)
{
    // TODO: spaces count as end of input with this IDE...
    
    bool validInput = true;
    
    array<string, 1> unaryOps_front { "-" }; // "log", "cos", "sin", "tan"
    array<const char, 1> unaryOps_back { '!' };
	
	const char ops[] = "*/+-";
	
	char *tmp = new char [expr.length()];
	strcpy(tmp, expr.c_str());
	
    char *p = tmp;
    double mult = 1.0;
    
    // check for unaryOps_front
    if (*tmp == '-') 
    {
        mult = -1.0;
        tmp++; // tmp + sizeof unaryOps_front
    }
    
    while (p = strpbrk(tmp, ops))
    { 
        opQ.push(*p);
        //*p = '\0';
        
        // check for unaryOps_back
        // apply it to stod(tmp)
        // then apply any unaryOps_front from last pass
        // ex: +cos3!- 
        //     cos op saved when processing +
        //     ! op applied first when processing -
        //     then cos applied
        
        try
        {
            numQ.push(stod(tmp)*mult);
            tmp = p+1; // p + sizeof unaryOps_back        
        }
        catch (const invalid_argument& e)
        {
            validInput = false;
        }
        catch (const out_of_range& e)
        {
            validInput = false;
        }    
            
        // check for unaryOps_front
        if (*tmp == '-') 
        {
            mult = -1.0;
            tmp++; // tmp + sizeof unaryOps_front
        }
        else
        {
            mult = 1.0;
        }
    }
    // ASSERT: Last char is "=", eg: tmp: 3= (or junk=)
    try
    {
        numQ.push(stod(tmp)*mult);  
    }
    catch (const invalid_argument& e)
    {
        validInput = false;
    }
    catch (const out_of_range& e)
    {
        validInput = false;
    }    
    
    return validInput;
}

bool GetInput(string &expression)
{
    bool quit = false;
	const string quitKeys[] = {"Q", "q"};
	const char evaluateToken = '=';
	
    cout << "\n\n>> ";
    cin >> expression;
    
    // just to echo user input for this weird input method...
    cout << expression; 
    
    for (auto &key : quitKeys)
    {
        if (expression == key)
        {
            quit = true;
            break;
        }
    }   
    
    if (!quit && expression.find(evaluateToken) != expression.length()-1)
    {
        cout << "\n\nInvalid input - missing or misplaced =\n";
        quit = true;
    }

    return !quit;
}

int main() 
{
    queue<char> opQ;
	queue<double> numQ;
	
	string expression;
	
    cout << "Hi!";
    
    while (GetInput(expression)) 
    {
        cout << " <working on it...>\n\n";
        
        // ASSERT: Last char is =
        if (!ParseExpression(expression, opQ, numQ))
        {
            cout << "\nInvalid input - Parse";
            return -1; //continue
        }        
    
        cout << "Ops: ";
        for (int i=0; i<opQ.size();i++)
        {
            cout << opQ.front() << " ";
            opQ.push(opQ.front());
            opQ.pop();
        }
        cout << "\nNums: ";
        for (int j=0; j<numQ.size();j++)
        {
            cout << numQ.front() << " ";
            numQ.push(numQ.front());
            numQ.pop();
        } 
        
        if (!Evaluate(opQ, numQ))
        {
            cout << "\n\nInvalid input - Evaluate";
            return -1; //continue
        }
        
        // ASSERT : numQ contains one element = TheResult
        cout << "\n\n" << expression << numQ.front();
        numQ.pop();
    }
    
	return 0;
}