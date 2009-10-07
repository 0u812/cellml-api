/* Model is correctly constrained.
 * No equations needed Newton-Raphson evaluation.
 * The rate and state arrays need 1 entries.
 * The algebraic variables array needs 0 entries.
 * The constant array needs 0 entries.
 * Variable storage is as follows:
 * * Target d^1/dt^1 x in component main
 * * * Variable type: algebraic variable
 * * * Variable index: 0
 * * * Variable storage: RATES[0]
 * * Target time in component main
 * * * Variable type: variable of integration
 * * * Variable index: 0
 * * * Variable storage: VOI
 * * Target x in component main
 * * * Variable type: algebraic variable
 * * * Variable index: 0
 * * * Variable storage: STATES[0]
 */
void SetupFixedConstants(double* CONSTANTS, double* RATES, double* STATES)
{
}
void EvaluateVariables(double VOI, double* CONSTANTS, double* RATES, double* STATES, double* ALGEBRAIC)
{
}
void ComputeRates(double VOI, double* STATES, double* RATES, double* CONSTANTS, double* ALGEBRAIC)
{
if (STATES[0]>10.0000)
{
  STATES[0] = 0.00000;
}
if (!(STATES[0]>10.0000))
{
  RATES[0] = 1.00000;
}
}
