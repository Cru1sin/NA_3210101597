
#include <iostream>
#include <functional>
#include <cmath>

const double PI = 3.14159265358979323846;
class EquationSolver {
public:
    virtual ~EquationSolver() = default;

    // Solve function that must be overridden in derived classes
    virtual double solve(std::function<double(double)> f, double x0, double x1, double tol) = 0;
};

class BisectionSolver : public EquationSolver {
public:
    double solve(std::function<double(double)> f, double a, double b, double tol) override{
        double c;
        if (f(a)*f(b) >= 0) {
            std::cerr << "Bisection method requires that f(a) and f(b) have opposite signs.\n";
            return NAN;
        }
        while ((b-a)/2.0 > tol )
        {
            c = (a+b)/2.0;
            if (f(c) == 0){
                return c;
            }
            else if (f(a)*f(c)<0){
                b = c;
            }
            else {
                a = c;
            }
        }
        return (a+b)/2.0;
    }
};

class NewtonSolver : public EquationSolver {
private:
    std::function<double(double)> df;  // Store the derivative function

public:
    // Constructor to accept the derivative function
    NewtonSolver(std::function<double(double)> dfFunc) : df(dfFunc) {}

    // Override solve function from EquationSolver
    double solve(std::function<double(double)> f, double x0, double /* unused */, double tol) override {
        double x = x0;
        int maxIterations = 100;
        int iter = 0;

        while (std::abs(f(x)) > tol && iter < maxIterations) {
            if (std::abs(df(x)) < 1e-8) {  // Avoid division by zero or small derivatives
                std::cerr << "Derivative too small!" << std::endl;
                break;
            }
            x = x - f(x) / df(x);
            iter++;
        }

        if (iter == maxIterations) {
            std::cerr << "Newton's method did not converge within the maximum number of iterations" << std::endl;
        }

        return x;
    }
};

class SecantSolver : public EquationSolver{
public:
    double solve(std::function<double(double)> f, double x0, double x1, double tol) override {
        double f0=f(x0), f1=f(x1);
        while (fabs(f1)>tol)
        {
            if (fabs(f1-f0)<1e-12)
            {
                std::cerr << "Secant method failed due to division by a small value.\n";
                return NAN;
            }
            double x2 = x1 - f1* (x1-x0)/(f1-f0);
            x0=x1;
            x1=x2;
            f0=f1;
            f1=f(x1);
        }
        return x1;
    }
};

// Example functions
double func1(double x) {
    return 1.0 / x - tan(x);  // Function: x^{-1} − tan(x)
}

double func2(double x) {
    return 1.0 / x - pow(2, x);  // Function: x^{-1} − 2^x
}

double func3(double x) {
    return pow(2, -x) + exp(x) + 2 * cos(x) - 6;  // Function: 2^{−x} + e^x + 2*cos(x) − 6
}

double func4(double x) {
    return (pow(x, 3) + 4 * pow(x, 2) + 3 * x + 5) / (2 * pow(x, 3) - 9 * pow(x, 2) + 18 * x - 2);  // Function: (x^3 +4x^2 +3x+5)/(2x^3 −9x^2 +18x−2)
}

double func_C(double x){
    return (x-tan(x));  //C. Function: x=tan(x)
}

double dfunc_C(double x){
    return (1-1/pow(cos(x),2));  //C. Derivative of Function x=tan(x): 1-1/(cos(x)^2)
}

double func_D1(double x){
    return sin(x/2)-1;
}

double func_D2(double x){
    return exp(x)-tan(x);
}

double func_D3(double x){
    return pow(x,3)-12*pow(x,2)+3*x+1;
}

// Function for volume calculation
double volumeFunction(double h) {
    double L = 10.0; // length in ft
    double r = 1.0;  // radius in ft
    double targetVolume = 12.4; // desired volume in ft^3

    return L * (0.5 * PI * r * r - r * r * asin(h / r) - h * sqrt(r * r - h * h)) - targetVolume;
}

// Derivative for Newton's method
double volumeDerivative(double h) {
    double r = 1.0; // radius in ft
    return -10 * ( r+ r * r )/ sqrt(r * r - h * h);
}


int main() {
    BisectionSolver bisection;
    SecantSolver secant;

    double tol = 1e-6;

    /* //====================================Problem A Test===================================================
    // Solve using Bisection method
    double bisection_result = bisection.solve(func, 0, 2, 1e-6);
    std::cout << "Bisection method result: " << bisection_result << "\n";

    // Solve using Newton's method
    NewtonSolver solver(dfunc);  // Pass the derivative function
    double root = solver.solve(func, 1.0, 0.0, 1e-6);  // x0 = 1.0, tol = 1e-6
    std::cout << "Newton's method result: " << root << std::endl;

    // Solve using Secant method
    double secant_result = secant.solve(func, 0, 2, tol);
    std::cout << "Secant method result: " << secant_result << "\n";
    */

     //====================================Problem B===================================================
    std::cout << "====================================Problem B===================================================" << std::endl;
    // Test case 1: x^{-1} − tan(x) on [0, π/2]
    double root1 = bisection.solve(func1, 0, PI / 2 - 0.1, tol);
    std::cout << "Root for B-func1: " << root1 << std::endl;

    // Test case 2: x^{-1} − 2^x on [0, 1]
    double root2 = bisection.solve(func2, 0, 1, tol);
    std::cout << "Root for B-func2: " << root2 << std::endl;

    // Test case 3: 2^{−x} + e^x + 2*cos(x) − 6 on [1,3]
    double root3 = bisection.solve(func3, 1, 3, tol);
    std::cout << "Root for B-func3: " << root3 << std::endl;

    // Test case 4: (x^3 +4x^2 +3x+5)/(2x^3 −9x^2 +18x−2) on [0,4]
    double root4 = bisection.solve(func4, 0, 4, tol);
    std::cout << "Root for B-func4: " << root4 << std::endl;
    

    //=======================================Problem C================================================
    //Problem C
    std::cout << "====================================Problem C===================================================" << std::endl;
    NewtonSolver solver_C(dfunc_C);  // Pass the derivative function
    //Choose x0 as 6 which is between 4.5 and 7.7
    
    double root_C = solver_C.solve(func_C, 4.5, 0.0, tol);  // tol = 1e-6
    std::cout << "Newton's method result near 4.5 is: " << root_C << std::endl;
    
    root_C = solver_C.solve(func_C, 7.7, 0.0, tol);  // tol = 1e-6
    std::cout << "Newton's method result near 7.7 is: " << root_C << std::endl;

    
    
    // ===========================================Problem D============================================ 
    std::cout << "====================================Problem D===================================================" << std::endl;
    //$sin(\frac{x}{2}) - 1$ with $x_0 = 0, x_1 = \frac{\pi}{2}$
    double secant_result = secant.solve(func_D1, PI/2, 4, tol);  
    std::cout << "Secant method result D1: " << secant_result << " with initial values of PI/2 and 4" << "\n";

    //$e^x-tanx$ with $x_0 =1,x_1 =1.4,$
    secant_result = secant.solve(func_D2, 1, 1.4, tol);
    std::cout << "Secant method result D2: " << secant_result << "\n";

    //$x^3-12x^2+3x+1$ with $x_0 =0,x_1 =-0.5.$
    secant_result = secant.solve(func_D3, 0, -0.5, tol);
    std::cout << "Secant method result D3: " << secant_result << "\n";
    
    
    // ===========================================Problem E============================================ 
    std::cout << "====================================Problem E===================================================" << std::endl;
    // Bisection method
    double rootBisection = bisection.solve(volumeFunction, 0, 1, tol);
    std::cout << "Bisection method root (depth of water): " << rootBisection << " ft" << std::endl;

    // Newton's method
    NewtonSolver solver(volumeDerivative);
    double rootNewton = solver.solve(volumeFunction, 0.5, 0.0, tol);
    std::cout << "Newton's method root (depth of water): " << rootNewton << " ft" << std::endl;

    // Secant method
    double rootSecant = secant.solve(volumeFunction, 0, 1, tol);
    std::cout << "Secant method root (depth of water): " << rootSecant << " ft" << std::endl;

    /* // ===========================================Problem F============================================ */
    std::cout << "====================================Problem F===================================================" << std::endl;
    double l = 89;
    double h = 49;
    double beta1 = 11.5 * PI / 180; // convert degrees to radians
    double D1 = 55;
    double D2 = 30;
    double A = l * sin(beta1);
    double B = l * cos(beta1);
    double C1 = (h + 0.5 * D1) * sin(beta1) - 0.5 * D1 * tan(beta1);
    double E1 = (h + 0.5 * D1) * cos(beta1) - 0.5 * D1;
    double C2 = (h + 0.5 * D2) * sin(beta1) - 0.5 * D2 * tan(beta1);
    double E2 = (h + 0.5 * D2) * cos(beta1) - 0.5 * D2;

    auto f1 = [A, B, C1, E1](double alpha) {
        return A * sin(alpha) * cos(alpha) + B * sin(alpha) * sin(alpha) - C1 * cos(alpha) - E1 * sin(alpha);
    };

    auto df1 = [A, B, C1, E1](double alpha) {
        return A * (cos(alpha) * cos(alpha) - sin(alpha) * sin(alpha)) + 2 * B * sin(alpha) * cos(alpha) + C1 * sin(alpha) - E1 * cos(alpha);
    };

    auto f2 = [A, B, C2, E2](double alpha) {
        return A * sin(alpha) * cos(alpha) + B * sin(alpha) * sin(alpha) - C2 * cos(alpha) - E2 * sin(alpha);
    };

    auto df2 = [A, B, C2, E2](double alpha) {
        return A * (cos(alpha) * cos(alpha) - sin(alpha) * sin(alpha)) + 2 * B * sin(alpha) * cos(alpha) + C2 * sin(alpha) - E2 * cos(alpha);
    };

    double initialGuess = 33 * PI / 180;

    NewtonSolver solver1(df1);
    double alpha1 = solver1.solve(f1, initialGuess, 0.0, tol);
    std::cout << "Alpha for D1 using Newton's method: " << alpha1 * 180 / PI << " degrees" << std::endl;

    
    NewtonSolver solver2(df2);
    double alpha2 = solver2.solve(f2, initialGuess, 0.0, tol);
    std::cout << "Alpha for D2 using Newton's method: " << alpha2 * 180 / PI << " degrees" << std::endl;
    
    double another_initialGuess = initialGuess + PI/4;
    double alpha3 = secant.solve(f2, initialGuess, another_initialGuess, tol); // initial guesses far apart
    std::cout << "Alpha for D2 using Secant method: " << alpha3 * 180 / PI << " degrees with " << another_initialGuess << " as another initial Guess" << std::endl;

    another_initialGuess = 100;
    alpha3 = secant.solve(f2, initialGuess, another_initialGuess, tol); // initial guesses far apart
    std::cout << "Alpha for D2 using Secant method: " << alpha3 * 180 / PI << " degrees with " << another_initialGuess << " as another initial Guess" << std::endl;


    return 0;
}