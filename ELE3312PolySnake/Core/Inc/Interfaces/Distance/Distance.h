/**
 * @file Distance.h
 * @date June 2025
 * @brief The file contains a class that describes an abstract interface to a distance measuring 
 * device
 * @defgroup ELE3312
 * @{
 */

#ifndef DISTANCE_H_
#define DISTANCE_H_


	/** @brief Describes an abstract interface to a distance measung device.
	*/
	class Distance{
		public:
			/** @brief Triggers a new measurement.
			 * @details This function triggers a new measurement and stores the 
			 * measurement result.
			 */
			virtual void measuer() = 0;

			/** @brief Returns the last measured distance.
			 * @retval The last measured distance.
			 */
			virtual float getDistance() const = 0;

			/** @brief Returns the minimum measurable distance.
			 * @details The method returns the minimal distance the device accepts. Measured distances 
			 * that are closer than the minimum distance are replaced by the smallest value measurable.
			 * @retval The minimum measurable distance.
			 */
			virtual float getMinDistance() const = 0;

			/** @brief Returns the maximum measurable distance.
			 * @details The method returns the maximum measurable distance. Distances that lie 
			 * outside of the maximum distance are replaced with the maximum value.
			 * @retval The maximum distance that is measurable by the device.
			 */
			virtual float getMaxDistance() const = 0;

			virtual void enableMeasurement() = 0;
			virtual void disableMeasurement() = 0;
	};



#endif /* DISTANCE_H_ */
